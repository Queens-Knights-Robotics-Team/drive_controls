/*
 * Copyright (c) 2020-2021 Advanced Robotics at the University of Washington <robomstr@uw.edu>
 *
 * This file is part of taproot-examples.
 *
 * taproot-examples is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * taproot-examples is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with taproot-examples.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "tap/algorithms/smooth_pid.hpp"
#include "tap/board/board.hpp"
#include "tap/architecture/periodic_timer.hpp"
#include <iostream>
#include "drivers_singleton.hpp"

// Motor Constructors
static constexpr tap::motor::MotorId MOTOR_ID = tap::motor::MOTOR6;
static constexpr tap::can::CanBus CAN_BUS = tap::can::CanBus::CAN_BUS1;


static constexpr int DESIRED_RPM = 3000;

tap::arch::PeriodicMilliTimer sendMotorTimeout(2);
tap::algorithms::SmoothPid pidController(20, 0, 0, 0, 8000, 1, 0, 1, 0);
tap::motor::DjiMotor motor(::DoNotUse_getDrivers(), MOTOR_ID, CAN_BUS, false, "cool motor");

tap::arch::PeriodicMilliTimer updateImuTimeout(2);

float yaw;

int main()
{
    /*
     * NOTE: We are using DoNotUse_getDrivers here because in the main
     *      robot loop we must access the singleton drivers to update
     *      IO states and run the scheduler.
     */
    ::Drivers *drivers = ::DoNotUse_getDrivers();

    Board::initialize();

    // Intialize Drivers
    drivers->can.initialize();
    drivers->mpu6500.init();

    // Initalize the motor
    motor.initialize();

    while (1)
    {
        // Read Data from driver
        drivers->mpu6500.read();

        if (sendMotorTimeout.execute())
        {
            // Update IMU's logic
            drivers->mpu6500.periodicIMUUpdate();

            // Calculate the amount of yaw required
            yaw = drivers->mpu6500.getGz();
            yaw = -yaw;

            // Apply PID Controller to the place of interest
            pidController.runControllerDerivateError(yaw - motor.getShaftRPM(), 1);

            // Send the PID adjusted desired output to the motor
            motor.setDesiredOutput(static_cast<int32_t>(pidController.getOutput()));

            // Sens
            drivers->djiMotorTxHandler.processCanSendData();
        }

        drivers->canRxHandler.pollCanData();
        modm::delay_us(10);
    }
    return 0;
}
