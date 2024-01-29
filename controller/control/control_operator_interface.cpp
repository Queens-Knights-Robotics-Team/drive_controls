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

#include "control_operator_interface.hpp"

#include "tap/algorithms/math_user_utils.hpp"
#include "tap/communication/serial/remote.hpp"

using tap::algorithms::limitVal;
using tap::communication::serial::Remote;

namespace control
{
ControlOperatorInterface::ControlOperatorInterface(Remote &remote) : remote(remote) {}

// STEP 2 (Tank Drive): Add getChassisTankLeftInput and getChassisTankRightInput function
// definitions
float ControlOperatorInterface::getChassisOmniLeftFrontInput() {
    float horizontal = limitVal(remote.getChannel(Remote::Channel::LEFT_HORIZONTAL), -1.0f, 1.0f);
    float vertical   = limitVal(remote.getChannel(Remote::Channel::LEFT_VERTICAL), -1.0f, 1.0f);
    float rightHorizontal = limitVal(remote.getChannel(Remote::Channel::RIGHT_HORIZONTAL), -1.0f, 1.0f);

    float result = vertical + horizontal + rightHorizontal;

    return result;
}

float ControlOperatorInterface::getChassisOmniLeftBackInput() {
    float horizontal  = limitVal(remote.getChannel(Remote::Channel::LEFT_HORIZONTAL), -1.0f, 1.0f);
    float vertical    = limitVal(remote.getChannel(Remote::Channel::LEFT_VERTICAL), -1.0f, 1.0f);
    float rightHorizontal = limitVal(remote.getChannel(Remote::Channel::RIGHT_HORIZONTAL), -1.0f, 1.0f);

    float result = vertical - horizontal + rightHorizontal;

    return result;
}

float ControlOperatorInterface::getChassisOmniRightFrontInput() {
    float horizontal = limitVal(remote.getChannel(Remote::Channel::LEFT_HORIZONTAL), -1.0f, 1.0f);
    float vertical   = limitVal(remote.getChannel(Remote::Channel::LEFT_VERTICAL), -1.0f, 1.0f);
    float rightHorizontal = limitVal(remote.getChannel(Remote::Channel::RIGHT_HORIZONTAL), -1.0f, 1.0f);

    float result = vertical - horizontal - rightHorizontal;

    return result;
}

float ControlOperatorInterface::getChassisOmniRightBackInput() {
    float horizontal = limitVal(remote.getChannel(Remote::Channel::LEFT_HORIZONTAL), -1.0f, 1.0f);
    float vertical   = limitVal(remote.getChannel(Remote::Channel::LEFT_VERTICAL), -1.0f, 1.0f);
    float rightHorizontal = limitVal(remote.getChannel(Remote::Channel::RIGHT_HORIZONTAL), -1.0f, 1.0f);

    float result = vertical + horizontal - rightHorizontal;

    return result;
}

}  // namespace control
