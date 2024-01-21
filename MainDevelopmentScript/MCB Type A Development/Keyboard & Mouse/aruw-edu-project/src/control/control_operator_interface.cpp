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

namespace control {

ControlOperatorInterface::ControlOperatorInterface(tap::communication::serial::Remote &remote)
    : remote(remote) {}

float ControlOperatorInterface::getChassisOmniLeftFrontInput() {
    float horizontal = remote.keyPressed(tap::communication::serial::Remote::Key::A) ? -1.0f :
                       remote.keyPressed(tap::communication::serial::Remote::Key::D) ? 1.0f : 0.0f;
    float vertical = remote.keyPressed(tap::communication::serial::Remote::Key::W) ? 1.0f :
                     remote.keyPressed(tap::communication::serial::Remote::Key::S) ? -1.0f : 0.0f;
    float mouseInput = static_cast<float>(remote.getMouseX());

    return vertical + horizontal + mouseInput;
}

float ControlOperatorInterface::getChassisOmniLeftBackInput() {
    float horizontal = remote.keyPressed(tap::communication::serial::Remote::Key::A) ? -1.0f :
                       remote.keyPressed(tap::communication::serial::Remote::Key::D) ? 1.0f : 0.0f;
    float vertical = remote.keyPressed(tap::communication::serial::Remote::Key::W) ? 1.0f :
                     remote.keyPressed(tap::communication::serial::Remote::Key::S) ? -1.0f : 0.0f;
    float mouseInput = static_cast<float>(remote.getMouseX());

    return vertical - horizontal + mouseInput;
}

float ControlOperatorInterface::getChassisOmniRightFrontInput() {
    float horizontal = remote.keyPressed(tap::communication::serial::Remote::Key::A) ? -1.0f :
                       remote.keyPressed(tap::communication::serial::Remote::Key::D) ? 1.0f : 0.0f;
    float vertical = remote.keyPressed(tap::communication::serial::Remote::Key::W) ? 1.0f :
                     remote.keyPressed(tap::communication::serial::Remote::Key::S) ? -1.0f : 0.0f;
    float mouseInput = static_cast<float>(remote.getMouseX());

    return vertical - horizontal - mouseInput;
}

float ControlOperatorInterface::getChassisOmniRightBackInput() {
    float horizontal = remote.keyPressed(tap::communication::serial::Remote::Key::A) ? -1.0f :
                       remote.keyPressed(tap::communication::serial::Remote::Key::D) ? 1.0f : 0.0f;
    float vertical = remote.keyPressed(tap::communication::serial::Remote::Key::W) ? 1.0f :
                     remote.keyPressed(tap::communication::serial::Remote::Key::S) ? -1.0f : 0.0f;
    float mouseInput = static_cast<float>(remote.getMouseX());

    return vertical + horizontal - mouseInput;
}

float ControlOperatorInterface::getChassisLeftVerticalInput() {
    return remote.keyPressed(tap::communication::serial::Remote::Key::W) ? 1.0f :
           remote.keyPressed(tap::communication::serial::Remote::Key::S) ? -1.0f : 0.0f;
}

float ControlOperatorInterface::getChassisRightVerticalInput() {
    return remote.keyPressed(tap::communication::serial::Remote::Key::UP_ARROW) ? 1.0f :
           remote.keyPressed(tap::communication::serial::Remote::Key::DOWN_ARROW) ? -1.0f : 0.0f;
}

float ControlOperatorInterface::getChassisLeftHorizontalInput() {
    return remote.keyPressed(tap::communication::serial::Remote::Key::A) ? -1.0f :
           remote.keyPressed(tap::communication::serial::Remote::Key::D) ? 1.0f : 0.0f;
}

float ControlOperatorInterface::getChassisRightHorizontalInput() {
    float mouseInput = static_cast<float>(remote.getMouseX());
    return mouseInput; // Using mouse X movement for right horizontal control
}

} // namespace control
