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

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "control/chassis/chassis_subsystem.hpp"
#include "control/chassis/chassis_tank_drive_command.hpp"

#include "drivers.hpp"

namespace control::chassis
{
static constexpr float MAX_OUT = ChassisTankDriveCommand::MAX_CHASSIS_SPEED_MPS;

class MockChassisSubsystem : public ChassisSubsystem
{
public:
    MockChassisSubsystem(Drivers& drivers) : ChassisSubsystem(drivers, {}) {}

    MOCK_METHOD(void, setVelocityTankDrive, (float, float), (override));
};

class ARUW_EDU_ChassisTankDriveCommand : public testing::Test
{
protected:
    ARUW_EDU_ChassisTankDriveCommand()
        : chassis(drivers),
          cmd(chassis, drivers.controlOperatorInterface)
    {
    }

    Drivers drivers;
    testing::StrictMock<MockChassisSubsystem> chassis;
    ChassisTankDriveCommand cmd;
};

TEST_F(ARUW_EDU_ChassisTankDriveCommand, end)
{
    EXPECT_CALL(chassis, setVelocityTankDrive(0, 0)).Times(2);

    cmd.end(true);
    cmd.end(false);
}

struct ChassisTankDriveCommandParameters
{
    float inLeft, outLeft;
    float inRight, outRight;
};

class ChassisTankDriveCommandParameterized
    : public ARUW_EDU_ChassisTankDriveCommand,
      public testing::WithParamInterface<ChassisTankDriveCommandParameters>
{
};

TEST_P(ChassisTankDriveCommandParameterized, execute)
{
    EXPECT_CALL(drivers.controlOperatorInterface, getChassisTankLeftInput)
        .WillOnce(testing::Return(GetParam().inLeft));
    EXPECT_CALL(drivers.controlOperatorInterface, getChassisTankRightInput)
        .WillOnce(testing::Return(GetParam().inRight));

    EXPECT_CALL(chassis, setVelocityTankDrive(GetParam().outLeft, GetParam().outRight));

    cmd.execute();
}

INSTANTIATE_TEST_SUITE_P(
    ARUW_EDU_ChassisTankDriveCommand,
    ChassisTankDriveCommandParameterized,
    testing::Values(
        ChassisTankDriveCommandParameters{.inLeft = 0, .outLeft = 0, .inRight = 0, .outRight = 0},
        ChassisTankDriveCommandParameters{
            .inLeft = -1,
            .outLeft = -MAX_OUT,
            .inRight = 1,
            .outRight = MAX_OUT},
        ChassisTankDriveCommandParameters{
            .inLeft = 1,
            .outLeft = MAX_OUT,
            .inRight = -1,
            .outRight = -MAX_OUT},
        ChassisTankDriveCommandParameters{
            .inLeft = 0.5,
            .outLeft = MAX_OUT / 2,
            .inRight = -0.5,
            .outRight = -MAX_OUT / 2},
        ChassisTankDriveCommandParameters{
            .inLeft = -1000,
            .outLeft = -MAX_OUT,
            .inRight = 1000,
            .outRight = MAX_OUT},
        ChassisTankDriveCommandParameters{
            .inLeft = 1000,
            .outLeft = MAX_OUT,
            .inRight = -1000,
            .outRight = -MAX_OUT}));
}  // namespace control::chassis
