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

// #include "tap/communication/serial/remote.hpp"
// #include "tap/communication/sensors/imu/mpu6500/mpu6500.hpp"

#include <tuple>

namespace tap::communication
{
namespace serial { class Remote; }
namespace sensors::imu::mpu6500 { class Mpu6500; }
}

namespace control
{
class ControlOperatorInterface
{
public:
    ControlOperatorInterface(tap::communication::serial::Remote& remote,
                             tap::communication::sensors::imu::mpu6500::Mpu6500& imu);

    std::tuple<double, double, double> pollInput();

    float getChassisOmniLeftFrontInput();
    float getChassisOmniLeftBackInput();
    float getChassisOmniRightFrontInput();
    float getChassisOmniRightBackInput();
private:
    tap::communication::serial::Remote& remote;
    tap::communication::sensors::imu::mpu6500::Mpu6500& imu;
};
}  // namespace control
