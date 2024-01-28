/*
 * Copyright (c) 2022 Advanced Robotics at the University of Washington <robomstr@uw.edu>
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

#include <gtest/gtest.h>

#include "control/algorithms/edu_pid.hpp"

namespace control::algorithms
{
static constexpr float KP = 10;
static constexpr float KI = 10;
static constexpr float KD = 10;
static constexpr float MAX_I = 100;
static constexpr float MAX_OUT = 1000;

static constexpr EduPidConfig PID_CONFIG{
    .kp = KP,
    .ki = KI,
    .kd = KD,
    .maxICumulative = MAX_I,
    .maxOutput = MAX_OUT,
};

TEST(EduPid, zero_error_zero_output)
{
    EduPid pid(PID_CONFIG);

    EXPECT_EQ(0, pid.runControllerDerivateError(0, 1));
}

TEST(EduPid, zero_dt_no_output)
{
    EduPid pid(PID_CONFIG);

    EXPECT_EQ(0, pid.runControllerDerivateError(1, 0));
}

TEST(EduPid, zero_max_out_zero_output)
{
    static constexpr EduPidConfig PID_CONFIG_ZERO_OUTPUT{
        .kp = KP,
        .ki = KI,
        .kd = KD,
        .maxICumulative = MAX_I,
        .maxOutput = 0,
    };

    EduPid pid(PID_CONFIG_ZERO_OUTPUT);

    EXPECT_EQ(0, pid.runControllerDerivateError(100, 100));
}

TEST(EduPid, error_proportional_gain_only_proportional_to_gain)
{
    static constexpr EduPidConfig PID_CONFIG_P_GAIN_ONLY{
        .kp = KP,
        .ki = 0,
        .kd = 0,
        .maxICumulative = 0,
        .maxOutput = MAX_OUT,
    };

    EduPid pid(PID_CONFIG_P_GAIN_ONLY);

    EXPECT_EQ(KP, pid.runControllerDerivateError(1, 1));
    EXPECT_EQ(10 * KP, pid.runControllerDerivateError(10, 1));
    EXPECT_EQ(-KP, pid.runControllerDerivateError(-1, 1));
    EXPECT_EQ(-10 * KP, pid.runControllerDerivateError(-10, 1));
}

static constexpr EduPidConfig PID_CONFIG_I_GAIN_ONLY{
    .kp = 0,
    .ki = KI,
    .kd = 0,
    .maxICumulative = MAX_I,
    .maxOutput = MAX_OUT,
};

TEST(EduPid, error_integral_gain_only_sums_error_over_time)
{
    EduPid pid(PID_CONFIG_I_GAIN_ONLY);

    EXPECT_EQ(2 * KI, pid.runControllerDerivateError(1, 2));
    EXPECT_EQ(4 * KI, pid.runControllerDerivateError(1, 2));
    EXPECT_EQ(6 * KI, pid.runControllerDerivateError(1, 2));
    EXPECT_EQ(4 * KI, pid.runControllerDerivateError(-1, 2));
    EXPECT_EQ(2 * KI, pid.runControllerDerivateError(-1, 2));
    EXPECT_EQ(0, pid.runControllerDerivateError(-1, 2));
    EXPECT_EQ(-2 * KI, pid.runControllerDerivateError(-1, 2));
    EXPECT_EQ(-4 * KI, pid.runControllerDerivateError(-1, 2));
    EXPECT_EQ(-6 * KI, pid.runControllerDerivateError(-1, 2));
}

TEST(EduPid, integral_limited_by_max_error_integral)
{
    EduPid pid(PID_CONFIG_I_GAIN_ONLY);

    EXPECT_EQ(MAX_I, pid.runControllerDerivateError(MAX_I, 2.0f));
}

TEST(EduPid, zero_i_accumulative_zero_integral_output)
{
    static constexpr EduPidConfig PID_OCNFIG_ZERO_I_SUM{
        .kp = 0,
        .ki = KI,
        .kd = 0,
        .maxICumulative = 0,
        .maxOutput = MAX_OUT,
    };

    EduPid pid(PID_OCNFIG_ZERO_I_SUM);

    EXPECT_EQ(0, pid.runControllerDerivateError(100, 100));
}

TEST(EduPid, error_derivative_gain_only_proportional_to_step_derivative_of_error)
{
    static constexpr EduPidConfig PID_CONFIG_D_GAIN_ONLY{
        .kp = 0,
        .ki = 0,
        .kd = KD,
        .maxICumulative = MAX_I,
        .maxOutput = MAX_OUT,
    };

    EduPid pid(PID_CONFIG_D_GAIN_ONLY);

    EXPECT_EQ(KD / 2.0f, pid.runControllerDerivateError(1, 2));
    EXPECT_EQ(10 * KD / 2.0f - KD / 2.0f, pid.runControllerDerivateError(10, 2));
}

TEST(EduPid, max_error_limited_by_max_output)
{
    EduPid pid(PID_CONFIG);

    EXPECT_EQ(MAX_OUT, pid.runControllerDerivateError(MAX_OUT, 2));
}
}  // namespace control::algorithms
