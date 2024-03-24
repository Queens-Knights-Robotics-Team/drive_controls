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

#include "standard.hpp"

#include "tap/util_macros.hpp"

#include "control/chassis/chassis_subsystem.hpp"
#include "control/chassis/chassis_tank_drive_command.hpp"

#include "drivers.hpp"

using tap::can::CanBus;
using tap::communication::serial::Remote;
using tap::control::RemoteMapState;
using tap::motor::MotorId;

namespace control
{
    Robot::Robot(Drivers &drivers)
        : drivers(drivers),
          // Construct ChassisSubsystem and ChassisTankDriveCommand
          chassis( // calling chassis constructor
              drivers,
              chassis::ChassisConfig{
                  // Add Gimbal Motor to chassis configuration
                  .gimbalId = MotorId::MOTOR6, // (added gimbal motor to chassis configuration)
                  .leftFrontId = MotorId::MOTOR2,
                  .leftBackId = MotorId::MOTOR3,
                  .rightBackId = MotorId::MOTOR4,
                  .rightFrontId = MotorId::MOTOR1,
                  .canBus = CanBus::CAN_BUS1,
                  .wheelVelocityPidConfig = modm::Pid<float>::Parameter(10, 0, 0, 0, 16'000),
              }),
          chassisTankDrive(chassis, drivers.controlOperatorInterface) // calling chassis tankdrive constructor
    {
    }

    void Robot::initSubsystemCommands()
    {
        initializeSubsystems();
        registerSoldierSubsystems();
        setDefaultSoldierCommands();
    }

    void Robot::initializeSubsystems()
    {
        // Initialize declared ChassisSubsystem
        chassis.initialize();
    }

    void Robot::registerSoldierSubsystems()
    {
        // Register declared ChassisSubsystem
        drivers.commandScheduler.registerSubsystem(&chassis);
    }

    void Robot::setDefaultSoldierCommands()
    {
        // Set ChassisTanKDriveCommand as default command for ChassisSubsystem
        chassis.setDefaultCommand(&chassisTankDrive);
    }

} // namespace control