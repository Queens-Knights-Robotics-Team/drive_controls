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

#include <bitset>

#include <gtest/gtest.h>

#include "control/standard.hpp"

#include "drivers.hpp"

using tap::control::CommandMapping;
using tap::control::HoldCommandMapping;
using tap::control::HoldRepeatCommandMapping;
using tap::control::Subsystem;

namespace control
{
class ARUW_EDU_Standard : public testing::Test
{
protected:
    ARUW_EDU_Standard() : robot(drivers){};

    Drivers drivers;

    Robot robot;
};

TEST_F(ARUW_EDU_Standard, Initialization)
{
    std::bitset<2> subsystems{};

    EXPECT_CALL(drivers.commandScheduler, registerSubsystem)
        .Times(2)
        .WillRepeatedly([&](Subsystem *sub) {
            if (dynamic_cast<chassis::ChassisSubsystem *>(sub) != nullptr)
            {
                subsystems.set(0);
            }
            else if (dynamic_cast<agitator::VelocityAgitatorSubsystem *>(sub) != nullptr)
            {
                subsystems.set(1);
            }
        });

    std::bitset<2> mappings{};

    EXPECT_CALL(drivers.commandMapper, addMap)
        .Times(2)
        .WillRepeatedly([&](CommandMapping *mapping) {
            if (dynamic_cast<HoldCommandMapping *>(mapping) != nullptr)
            {
                mappings.set(0);
            }
            else if (dynamic_cast<HoldRepeatCommandMapping *>(mapping) != nullptr)
            {
                mappings.set(1);
            }
        });

    robot.initSubsystemCommands();

    EXPECT_EQ(2, subsystems.count());
    EXPECT_EQ(2, mappings.count());
}
}  // namespace control
