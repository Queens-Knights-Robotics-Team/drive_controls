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

#include <memory>

#include <gtest/gtest.h>

#include "control/chassis/chassis_subsystem.hpp"

#include "drivers.hpp"

using tap::can::CanBus;
using tap::motor::MotorId;
using testing::AtLeast;
using testing::Return;

namespace control::chassis
{
static constexpr float MAX_OUT = 1;
static constexpr int LF = int(ChassisSubsystem::MotorId::LF);
static constexpr int LB = int(ChassisSubsystem::MotorId::LB);
static constexpr int RB = int(ChassisSubsystem::MotorId::RB);
static constexpr int RF = int(ChassisSubsystem::MotorId::RF);
static constexpr float MAX_WHEELSPEED = ChassisSubsystem::MAX_WHEELSPEED_RPM;

class TestableChassisSubsystem : public ChassisSubsystem
{
public:
    using ChassisSubsystem::ChassisSubsystem;

    std::array<Motor, static_cast<uint8_t>(MotorId::NUM_MOTORS)> &GetMotors() { return motors; }
};

class ARUW_EDU_ChassisSubsystem : public testing::Test
{
protected:
    const ChassisConfig DEFAULT_CHASSIS_CONFIG = {
        .leftFrontId = MotorId::MOTOR1,
        .leftBackId = MotorId::MOTOR2,
        .rightBackId = MotorId::MOTOR3,
        .rightFrontId = MotorId::MOTOR4,
        .canBus = CanBus::CAN_BUS1,
        .wheelVelocityPidConfig = {1, 0, 0, 0, MAX_OUT},
    };

    ARUW_EDU_ChassisSubsystem()
        : chassis(new TestableChassisSubsystem(drivers, DEFAULT_CHASSIS_CONFIG))
    {
    }

    Drivers drivers;

    std::shared_ptr<TestableChassisSubsystem> chassis;
};

TEST_F(ARUW_EDU_ChassisSubsystem, Initialize)
{
    for (auto &motor : chassis->GetMotors())
    {
        EXPECT_CALL(motor, initialize);
    }

    chassis->initialize();
}

TEST_F(ARUW_EDU_ChassisSubsystem, MotorsInvertedCorrectly)
{
    EXPECT_FALSE(chassis->GetMotors()[LF].DjiMotor::isMotorInverted());
    EXPECT_FALSE(chassis->GetMotors()[LB].DjiMotor::isMotorInverted());
    EXPECT_TRUE(chassis->GetMotors()[RB].DjiMotor::isMotorInverted());
    EXPECT_TRUE(chassis->GetMotors()[RF].DjiMotor::isMotorInverted());
}

struct ChassisSubsystemParams
{
    float left{}, right{};
    float lfActualRpm{}, lbActualRpm{}, rbActualRpm{}, rfActualRpm{};
    float lfExpectedOut{}, lbExpectedOut{}, rbExpectedOut{}, rfExpectedOut{};
};

std::ostream &operator<<(std::ostream &stream, const ChassisSubsystemParams &o)
{
    stream << "{" << o.left << ", " << o.right << ", " << o.lfActualRpm << ", " << o.lbActualRpm
           << ", " << o.rbActualRpm << ", " << o.rfActualRpm << ", " << o.lfExpectedOut << ", "
           << o.lbExpectedOut << ", " << o.rbExpectedOut << ", " << o.rfExpectedOut << "}";
    return stream;
}

class ChassisSubsystemParameterized : public ARUW_EDU_ChassisSubsystem,
                                      public testing::WithParamInterface<ChassisSubsystemParams>
{
};

TEST_P(ChassisSubsystemParameterized, SetVelocityTankDrive)
{
    const ChassisSubsystemParams &p = GetParam();

    EXPECT_CALL(chassis->GetMotors()[LF], setDesiredOutput(p.lfExpectedOut));
    EXPECT_CALL(chassis->GetMotors()[LB], setDesiredOutput(p.lbExpectedOut));
    EXPECT_CALL(chassis->GetMotors()[RB], setDesiredOutput(p.rbExpectedOut));
    EXPECT_CALL(chassis->GetMotors()[RF], setDesiredOutput(p.rfExpectedOut));

    EXPECT_CALL(chassis->GetMotors()[LF], getShaftRPM)
        .Times(AtLeast(1))
        .WillRepeatedly(Return(p.lfActualRpm));
    EXPECT_CALL(chassis->GetMotors()[LB], getShaftRPM)
        .Times(AtLeast(1))
        .WillRepeatedly(Return(p.lbActualRpm));
    EXPECT_CALL(chassis->GetMotors()[RB], getShaftRPM)
        .Times(AtLeast(1))
        .WillRepeatedly(Return(p.rbActualRpm));
    EXPECT_CALL(chassis->GetMotors()[RF], getShaftRPM)
        .Times(AtLeast(1))
        .WillRepeatedly(Return(p.rfActualRpm));

    chassis->setVelocityTankDrive(p.left, p.right);

    chassis->refresh();
}

static constexpr ChassisSubsystemParams ZERO_TARGET_ZERO_ACTUAL_RPM{};
static constexpr ChassisSubsystemParams ZERO_TARGET_NONZERO_ACTUAL_RPM{
    .lfActualRpm = 1'000,
    .lbActualRpm = -1'000,
    .rbActualRpm = 1'000,
    .rfActualRpm = -1'000,

    .lfExpectedOut = -MAX_OUT,
    .lbExpectedOut = MAX_OUT,
    .rbExpectedOut = -MAX_OUT,
    .rfExpectedOut = MAX_OUT,
};
static constexpr ChassisSubsystemParams POSITIVE_TARGET_ZERO_WHEELSPEED{
    .left = 1,
    .right = 0.5,

    .lfActualRpm = 0,
    .lbActualRpm = 0,
    .rbActualRpm = 0,
    .rfActualRpm = 0,

    .lfExpectedOut = MAX_OUT,
    .lbExpectedOut = MAX_OUT,
    .rbExpectedOut = MAX_OUT,
    .rfExpectedOut = MAX_OUT,
};
static constexpr ChassisSubsystemParams NEGATIVE_TARGET_ZERO_WHEELSPEED{
    .left = -0.5,
    .right = -1,

    .lfActualRpm = 0,
    .lbActualRpm = 0,
    .rbActualRpm = 0,
    .rfActualRpm = 0,

    .lfExpectedOut = -MAX_OUT,
    .lbExpectedOut = -MAX_OUT,
    .rbExpectedOut = -MAX_OUT,
    .rfExpectedOut = -MAX_OUT,
};
static constexpr ChassisSubsystemParams NONZERO_TARGET_CORRECT_WHEELSPEED{
    .left = -1,
    .right = 1,

    .lfActualRpm = -4774.65,
    .lbActualRpm = -4774.65,
    .rbActualRpm = 4774.65,
    .rfActualRpm = 4774.65,
};
static constexpr ChassisSubsystemParams DESIRED_OVER_MAX{
    .left = 10000,
    .right = -10000,

    .lfActualRpm = MAX_WHEELSPEED,
    .lbActualRpm = MAX_WHEELSPEED,
    .rbActualRpm = -MAX_WHEELSPEED,
    .rfActualRpm = -MAX_WHEELSPEED,
};

INSTANTIATE_TEST_SUITE_P(
    ARUW_EDU_ChassisSubsystem,
    ChassisSubsystemParameterized,
    testing::Values(
        ZERO_TARGET_ZERO_ACTUAL_RPM,
        ZERO_TARGET_NONZERO_ACTUAL_RPM,
        POSITIVE_TARGET_ZERO_WHEELSPEED,
        NEGATIVE_TARGET_ZERO_WHEELSPEED,
        NONZERO_TARGET_CORRECT_WHEELSPEED,
        DESIRED_OVER_MAX));
}  // namespace control::chassis
