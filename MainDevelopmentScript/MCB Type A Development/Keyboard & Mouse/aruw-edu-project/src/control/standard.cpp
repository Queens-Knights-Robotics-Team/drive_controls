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
      // STEP 3 (Tank Drive): construct ChassisSubsystem and ChassisTankDriveCommand
    chassis( // calling chassis constructor
        drivers,
        chassis::ChassisConfig{
            .leftFrontId = MotorId::MOTOR2,
            .leftBackId = MotorId::MOTOR3,
            .rightBackId = MotorId::MOTOR4,
            .rightFrontId = MotorId::MOTOR1,
            .canBus = CanBus::CAN_BUS1,
            .wheelVelocityPidConfig = modm::Pid<float>::Parameter(10, 0, 0, 0, 16'000),
        }),
    chassisTankDrive(chassis, drivers.controlOperatorInterface)// calling chassis tankdrive constructor
    
// STEP 3 (Agitator Control): construct VelocityAgitatorSubsystem and MoveIntegralCommand
// STEP 8 (Agitator Control): construct HoldRepeatCommandMapping and HoldCommandMapping
{
}

void Robot::initSubsystemCommands()
{
    initializeSubsystems();
    registerSoldierSubsystems();
    setDefaultSoldierCommands();
    startSoldierCommands();
    registerSoldierIoMappings();
}

void Robot::initializeSubsystems()
{
    // STEP 4 (Tank Drive): initialize declared ChassisSubsystem
    chassis.initialize();
    // STEP 4 (Agitator Control): initialize declared VelocityAgitatorSubsystem
    //agitatorSubsystem.initialize();
}

void Robot::registerSoldierSubsystems()
{
    // STEP 5 (Tank Drive): register declared ChassisSubsystem
    drivers.commandScheduler.registerSubsystem(&chassis);


    // STEP 5 (Agitator Control): register declared VelocityAgitatorSubsystem
}

void Robot::setDefaultSoldierCommands()
{
    // STEP 6 (Tank Drive): set ChassisTanKDriveCommand as default command for ChassisSubsystem
    chassis.setDefaultCommand(&chassisTankDrive);


}

void Robot::startSoldierCommands() {}

void Robot::registerSoldierIoMappings()
{
    // STEP 9 (Agitator Control): register HoldRepeatCommandMapping and HoldCommandMapping
}
}  // namespace control