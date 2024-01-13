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

using tap::algorithms::limitVal;

namespace control::chassis
{
// Create constructor
ChassisSubsystem::ChassisSubsystem(Drivers &drivers, const ChassisConfig &config) // same btwn tank and mecanum
    : tap::control::Subsystem(&drivers),
      desiredOutput{},
      pidControllers{},
      motors{
          Motor(&drivers, config.leftFrontId, config.canBus, false, "LF"),
          Motor(&drivers, config.leftBackId, config.canBus, false, "LB"),
          Motor(&drivers, config.rightBackId, config.canBus, true, "RF"),
          Motor(&drivers, config.rightFrontId, config.canBus, true, "RB"),
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

// STEP 5 Refresh function
void ChassisSubsystem::refresh() // same btwn tank and mecanum
{
    auto runPid = [](Pid &pid, Motor &motor, float desiredOutput) {
        pid.update(desiredOutput - motor.getShaftRPM());
        motor.setDesiredOutput(pid.getValue());
    };

    for (size_t ii = 0; ii < motors.size(); ii++)
    {
        runPid(pidControllers[ii], motors[ii], desiredOutput[ii]);
    }
}
}  // namespace control::chassis