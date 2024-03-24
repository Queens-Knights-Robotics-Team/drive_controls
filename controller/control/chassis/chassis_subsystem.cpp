/*
 * Copyright (c) 2020-2022 Advanced Robotics at the University of Washington <robomstr@uw.edu>
 *
 * This file is part of aruw-edu.
 *
 * aruw-edu is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * aruw-edu is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with aruw-edu.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "chassis_subsystem.hpp"

#include "tap/algorithms/math_user_utils.hpp"

#include "drivers.hpp"

#include "tap/algorithms/smooth_pid.hpp" // added for gimbal, should be included from main.cpp but gives error

using tap::algorithms::limitVal;

// stuff from GimbalTrackingIMU main.cpp 
static constexpr int DESIRED_RPM = 3000;
tap::arch::PeriodicMilliTimer sendMotorTimeout(2);
tap::algorithms::SmoothPid pidController(20, 0, 0, 0, 8000, 1, 0, 1, 0);
tap::arch::PeriodicMilliTimer updateImuTimeout(2);

namespace control::chassis
{
    float yaw; // added for Gimbal
    // Create constructor
    ChassisSubsystem::ChassisSubsystem(Drivers &drivers, const ChassisConfig &config) // same btwn tank and mecanum
        : tap::control::Subsystem(&drivers),
          desiredOutput{},
          pidControllers{},
          motors{
              // Add Gimbal Motor Intilaization (done)
              Motor(&drivers, config.gimbalId, config.canBus, false, "GIMBAL"), //added gimbal motor init, see DoNotUse_getDrivers()?
              Motor(&drivers, config.leftFrontId, config.canBus, false, "LF"),
              Motor(&drivers, config.leftBackId, config.canBus, false, "LB"),
              Motor(&drivers, config.rightBackId, config.canBus, true, "RB"),
              Motor(&drivers, config.rightFrontId, config.canBus, true, "RF"),
          }
    {
        for (auto &controller : pidControllers)
        {
            controller.setParameter(config.wheelVelocityPidConfig);
        }
    }

    // Initialize function
    void ChassisSubsystem::initialize() // same btwn tank and mecanum
    {
        for (auto &motor : motors)
        {
            motor.initialize();
        }
    }

    // setVelocity function
    void ChassisSubsystem::setVelocity(float leftVert, float rightVert, float leftHorz, float rightHorz) // same btwn tank and mecanum
    {
        leftVert = mpsToRpm(leftVert);
        rightVert = mpsToRpm(rightVert);
        leftHorz = mpsToRpm(leftHorz);
        rightHorz = mpsToRpm(rightHorz);

        leftVert = limitVal(leftVert, -MAX_WHEELSPEED_RPM, MAX_WHEELSPEED_RPM);
        rightVert = limitVal(rightVert, -MAX_WHEELSPEED_RPM, MAX_WHEELSPEED_RPM);
        leftHorz = limitVal(leftHorz, -MAX_WHEELSPEED_RPM, MAX_WHEELSPEED_RPM);
        rightHorz = limitVal(rightHorz, -MAX_WHEELSPEED_RPM, MAX_WHEELSPEED_RPM);

        // Implment Mecanum Wheel Logical Code Here

        desiredOutput[static_cast<uint8_t>(MotorId::LF)] = leftVert + leftHorz + rightHorz;
        desiredOutput[static_cast<uint8_t>(MotorId::LB)] = leftVert - leftHorz + rightHorz;
        desiredOutput[static_cast<uint8_t>(MotorId::RF)] = leftVert - leftHorz - rightHorz;
        desiredOutput[static_cast<uint8_t>(MotorId::RB)] = leftVert + leftHorz - rightHorz;
    }

    //setGimbal function (need to figure out the problem with motor reference!)
    void ChassisSubsystem::setGimbal()
    {
         drivers->mpu6500.read();

        if (sendMotorTimeout.execute())
        {
            // Update IMU's logic
            drivers->mpu6500.periodicIMUUpdate();

            // Calculate the amount of yaw required
            yaw = drivers->mpu6500.getGz();
            yaw = -yaw;

            // Apply PID Controller to the place of interest
            pidController.runControllerDerivateError(yaw - MotorId::GIMBAL.getShaftRPM(), 1); // error here, has to do with motor type!

            // Send the PID adjusted desired output to the motor
            MotorId::GIMBAL.setDesiredOutput(static_cast<int32_t>(pidController.getOutput()));

            // Sens
            drivers->djiMotorTxHandler.processCanSendData();
        }

        drivers->canRxHandler.pollCanData();
        modm::delay_us(10);
    }

    // STEP 5 Refresh function
    void ChassisSubsystem::refresh() // same btwn tank and mecanum
    {
        auto runPid = [](Pid &pid, Motor &motor, float desiredOutput)
        {
            pid.update(desiredOutput - motor.getShaftRPM());
            motor.setDesiredOutput(pid.getValue());
        };

        for (size_t ii = 0; ii < motors.size(); ii++)
        {
            runPid(pidControllers[ii], motors[ii], desiredOutput[ii]);
        }
    }
} // namespace control::chassis