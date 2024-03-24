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
#include "tap/architecture/clock.hpp"

using tap::algorithms::limitVal;
using tap::communication::serial::Remote;

namespace control
{

// static constexpr double R = 0.07625;    // radius of mechanum wheels in metres
static constexpr double Scale = 1.0;    // arbitrary scaling factor for testing

ControlOperatorInterface::ControlOperatorInterface(Remote &remote) : remote(remote) {}

std::tuple<double, double, double> ControlOperatorInterface::pollInput() {
    /* use doubles for enhanced precision when processing return values */

    /* setup input variables to be processed */
    double lh = static_cast<double>(std::clamp(remote.getChannel(Remote::Channel::LEFT_HORIZONTAL), -1.0f, 1.0f)) * Scale;
    double lv = static_cast<double>(std::clamp(remote.getChannel(Remote::Channel::LEFT_VERTICAL), -1.0f, 1.0f)) * Scale;
    double rh = static_cast<double>(std::clamp(remote.getChannel(Remote::Channel::RIGHT_HORIZONTAL), -1.0, 1.0f)) * Scale;

    return std::make_tuple(lh, lv, rh);
}

/* field-centric movement strategy
 * 
 * - keep track of the chassis' rotation using IMU
 * double botHeading = imu.getRobotYawPitchRollAngles().getYaw(AngleUnit.RADIANS);
 * 
 * - rotate the movement direction counter to the chassis' rotation
 * double rotX = x * Math.cos(-botHeading) - y * Math.sin(-botHeading);
 * double rotY = x * Math.sin(-botHeading) + y * Math.cos(-botHeading);
 * 
 * - how to combine variables for proper movement
 * double denominator = Math.max(Math.abs(rotY) + Math.abs(rotX) + Math.abs(rx), 1);
 * double frontLeftPower = (rotY + rotX + rx) / denominator;
 * double backLeftPower = (rotY - rotX + rx) / denominator;
 * double frontRightPower = (rotY - rotX - rx) / denominator;
 * double backRightPower = (rotY + rotX - rx) / denominator;
*/

float ControlOperatorInterface::getChassisOmniLeftFrontInput() {
    auto [vx, vy, w] = pollInput();
    double denom = std::max(std::abs(vy) + std::abs(vx) + std::abs(w), static_cast<double>(1.0));
    return (vy + vx + w) / denom;
}

float ControlOperatorInterface::getChassisOmniLeftBackInput() {
    auto [vx, vy, w] = pollInput();
    double denom = std::max(std::abs(vy) + std::abs(vx) + std::abs(w), static_cast<double>(1.0));
    return (vy - vx + w) / denom;
}

float ControlOperatorInterface::getChassisOmniRightFrontInput() {
    auto [vx, vy, w] = pollInput();
    double denom = std::max(std::abs(vy) + std::abs(vx) + std::abs(w), static_cast<double>(1.0));
    return (vy - vx - w) / denom;
}

float ControlOperatorInterface::getChassisOmniRightBackInput() {
    auto [vx, vy, w] = pollInput();
    double denom = std::max(std::abs(vy) + std::abs(vx) + std::abs(w), static_cast<double>(1.0));
    return (vy + vx - w) / denom;
}

}  // namespace control
