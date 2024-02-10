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

#include "chassis_tank_drive_command.hpp"

#include "tap/algorithms/math_user_utils.hpp"

#include "control/control_operator_interface.hpp"

#include "chassis_subsystem.hpp"

using tap::algorithms::limitVal;

namespace control::chassis
{
// Constructor
ChassisTankDriveCommand::ChassisTankDriveCommand( // right of :: means method (constructor here), left is class that contains it
    ChassisSubsystem &chassis, // ? about &
    ControlOperatorInterface &operatorInterface)
    : chassis(chassis), // calling constructor on chassis parameter
      operatorInterface(operatorInterface)
{
    addSubsystemRequirement(&chassis);
}

// Execute function
void ChassisTankDriveCommand::execute()
{
    auto scale = [](float raw) -> float {
        return limitVal(raw, -1.0f, 1.0f) * MAX_CHASSIS_SPEED_MPS;
    };

    chassis.setVelocity(
        scale(operatorInterface.getChassisLeftVerticalInput()),
        scale(operatorInterface.getChassisRightVerticalInput()),
        scale(operatorInterface.getChassisLeftHorizontalInput()), // added this for left horizontal joystick
        scale(operatorInterface.getChassisRightHorizontalInput()) // added this for right horizontal joystick
        );
}

// STEP 3 (Tank Drive): end function
void ChassisTankDriveCommand::end(bool) { chassis.setVelocity(0, 0, 0, 0); }
};  // namespace control::chassis