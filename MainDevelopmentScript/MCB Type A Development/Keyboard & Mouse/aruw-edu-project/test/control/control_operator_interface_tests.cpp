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

#include <functional>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "tap/mock/remote_mock.hpp"

#include "control/control_operator_interface.hpp"

#include "drivers.hpp"

using tap::mock::RemoteMock;
using testing::AtLeast;
using testing::Return;

namespace control
{
class ARUW_EDU_ControlOperatorInterface : public testing::Test
{
protected:
    ARUW_EDU_ControlOperatorInterface() : remote(&drivers), aut(remote) {}

    Drivers drivers;

    testing::StrictMock<RemoteMock> remote;

    ControlOperatorInterface aut;
};

struct GetterParams
{
    std::function<void(RemoteMock &)> defineRemoteExpectations;
    std::function<float(ControlOperatorInterface &)> callGetter;
    float expectedResult{};
};

std::ostream &operator<<(std::ostream &stream, const GetterParams &params)
{
    stream << "{" << &params.defineRemoteExpectations << ", " << &params.callGetter << ", "
           << params.expectedResult << "}";
    return stream;
}

class GetterParameterized : public ARUW_EDU_ControlOperatorInterface,
                            public testing::WithParamInterface<GetterParams>
{
protected:
    void SetUp() override { GetParam().defineRemoteExpectations(remote); }
};

TEST_P(GetterParameterized, GetChassisTankLeftInput)
{
    EXPECT_EQ(GetParam().expectedResult, GetParam().callGetter(aut));
}

template <int num, int denom, RemoteMock::Channel Ch>
static void expectChannel(RemoteMock &remote)
{
    EXPECT_CALL(remote, getChannel(Ch))
        .Times(AtLeast(1))
        .WillOnce(Return(static_cast<float>(num) / static_cast<float>(denom)));
}

template <int num, int denom = 100>
static void expectLeftVertical(RemoteMock &remote)
{
    expectChannel<num, denom, RemoteMock::Channel::LEFT_VERTICAL>(remote);
}

template <int num, int denom = 100>
static void expectRightVertical(RemoteMock &remote)
{
    expectChannel<num, denom, RemoteMock::Channel::RIGHT_VERTICAL>(remote);
}

static float getChassisTankLeftInput(ControlOperatorInterface &o)
{
    return o.getChassisTankLeftInput();
}

static float getChassisTankRightInput(ControlOperatorInterface &o)
{
    return o.getChassisTankRightInput();
}

INSTANTIATE_TEST_SUITE_P(
    ARUW_EDU_ControlOperatorInterface,
    GetterParameterized,
    testing::Values(
        GetterParams{
            .defineRemoteExpectations = expectLeftVertical<0>,
            .callGetter = getChassisTankLeftInput},
        GetterParams{
            .defineRemoteExpectations = expectLeftVertical<10>,
            .callGetter = getChassisTankLeftInput,
            .expectedResult = 0.1},
        GetterParams{
            .defineRemoteExpectations = expectLeftVertical<50>,
            .callGetter = getChassisTankLeftInput,
            .expectedResult = 0.5},
        GetterParams{
            .defineRemoteExpectations = expectLeftVertical<100>,
            .callGetter = getChassisTankLeftInput,
            .expectedResult = 1},
        GetterParams{
            .defineRemoteExpectations = expectLeftVertical<-75>,
            .callGetter = getChassisTankLeftInput,
            .expectedResult = -0.75},
        GetterParams{
            .defineRemoteExpectations = expectLeftVertical<100000>,
            .callGetter = getChassisTankLeftInput,
            .expectedResult = 1},
        GetterParams{
            .defineRemoteExpectations = expectLeftVertical<-1000000>,
            .callGetter = getChassisTankLeftInput,
            .expectedResult = -1},
        GetterParams{
            .defineRemoteExpectations = expectRightVertical<0>,
            .callGetter = getChassisTankRightInput},
        GetterParams{
            .defineRemoteExpectations = expectRightVertical<10>,
            .callGetter = getChassisTankRightInput,
            .expectedResult = 0.1},
        GetterParams{
            .defineRemoteExpectations = expectRightVertical<50>,
            .callGetter = getChassisTankRightInput,
            .expectedResult = 0.5},
        GetterParams{
            .defineRemoteExpectations = expectRightVertical<100>,
            .callGetter = getChassisTankRightInput,
            .expectedResult = 1},
        GetterParams{
            .defineRemoteExpectations = expectRightVertical<-75>,
            .callGetter = getChassisTankRightInput,
            .expectedResult = -0.75},
        GetterParams{
            .defineRemoteExpectations = expectRightVertical<100000>,
            .callGetter = getChassisTankRightInput,
            .expectedResult = 1},
        GetterParams{
            .defineRemoteExpectations = expectRightVertical<-1000000>,
            .callGetter = getChassisTankRightInput,
            .expectedResult = -1}));
}  // namespace control
