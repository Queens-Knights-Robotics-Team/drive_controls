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
#include "tap/communication/sensors/imu/mpu6500/mpu6500.hpp"
#include "tap/architecture/clock.hpp"

using tap::algorithms::limitVal;
using tap::communication::serial::Remote;
using tap::communication::sensors::imu::mpu6500::Mpu6500;

namespace control
{

#define PI 3.1415927f

ControlOperatorInterface::ControlOperatorInterface(Remote &remote, Mpu6500& imu)
        : remote(remote), imu(imu) {}

std::tuple<double, double, double> ControlOperatorInterface::pollInput() {
    /* use doubles for enhanced precision when processing return values */
    double x    = static_cast<double>(std::clamp(remote.getChannel(Remote::Channel::LEFT_HORIZONTAL),  -1.0f, 1.0f));
    double y    = static_cast<double>(std::clamp(remote.getChannel(Remote::Channel::LEFT_VERTICAL),    -1.0f, 1.0f));
    double yaw  = static_cast<double>(modm::toRadian(imu.getYaw()));
    double rotX = x * std::cos(-yaw) - y * std::sin(-yaw);
    double rotY = x * std::sin(-yaw) + y * std::cos(-yaw);

    double rx   = static_cast<double>(std::clamp(remote.getChannel(Remote::Channel::RIGHT_HORIZONTAL), -1.0f, 1.0f));
    
    return std::make_tuple(rotX, rotY, rx);
}

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
