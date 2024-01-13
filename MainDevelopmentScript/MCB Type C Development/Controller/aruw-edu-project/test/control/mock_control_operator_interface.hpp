/*
 * Copyright (c) 2020-2021 Advanced Robotics at the University of Washington <robomstr@uw.edu>
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

#include <gmock/gmock.h>

#include "control/control_operator_interface.hpp"

namespace control
{
class MockControlOperatorInterface : public ControlOperatorInterface
{
public:
    MockControlOperatorInterface(tap::communication::serial::Remote &remote)
        : ControlOperatorInterface(remote)
    {
    }

    MOCK_METHOD(float, getChassisTankLeftInput, (), (override));
    MOCK_METHOD(float, getChassisTankRightInput, (), (override));
};
}  // namespace control
