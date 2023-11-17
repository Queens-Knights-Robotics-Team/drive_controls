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

#pragma once

#include "tap/control/hold_command_mapping.hpp"
#include "tap/control/hold_repeat_command_mapping.hpp"
#include "tap/control/setpoint/commands/move_integral_command.hpp"

#include "control/agitator/velocity_agitator_subsystem.hpp"
#include "control/chassis/chassis_subsystem.hpp"
#include "control/chassis/chassis_tank_drive_command.hpp"

class Drivers;

namespace control
{
class Robot
{
public:
    Robot(Drivers &drivers);

    void initSubsystemCommands();

private:
    void initializeSubsystems();
    void registerSoldierSubsystems();
    void setDefaultSoldierCommands();
    void startSoldierCommands();
    void registerSoldierIoMappings();

    Drivers &drivers;

    // STEP 1 (Tank Drive): declare ChassisSubystem

    // STEP 2 (Tank Drive): declare ChassisTankDriveCommand

    // STEP 1 (Agitator Control): declare VelocityAgitatorSubsystem

    // STEP 2 (Agitator Control): declare MoveIntegralCommand

    // STEP 6 (Agitator Control): declare HoldRepeatCommandMapping (rightSwitchUp)

    // STEP 7 (Agitator Control): declare HoldCommandMapping (leftMousePressed)
};
}  // namespace control
