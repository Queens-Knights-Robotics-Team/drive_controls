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

#include "tap/control/command.hpp"

namespace control
{
class ControlOperatorInterface;
}

namespace control::chassis
{
class ChassisSubsystem;

/**
 * @brief Commands a ChassisSubsystem using tank drive. The left and right sides of the chassis are
 * commanded independently by this command. This command executes constantly, taking remote inputs
 * in from a control operator interface, transforming remote input into chassis speed.
 */
class ChassisTankDriveCommand : public tap::control::Command
{
public:
    static constexpr float MAX_CHASSIS_SPEED_MPS = 3.0f;

    /**
     * @brief Construct a new Chassis Tank Drive Command object
     *
     * @param chassis Chassis to control.
     */
    ChassisTankDriveCommand(ChassisSubsystem &chassis, ControlOperatorInterface &operatorInterface);

    const char *getName() const override { return "Chassis tank drive"; }

    void initialize() override {}

    void execute() override;

    void end(bool interrupted) override;

    bool isFinished() const { return false; }

private:
    ChassisSubsystem &chassis;

    ControlOperatorInterface &operatorInterface;
};
}  // namespace control::chassis