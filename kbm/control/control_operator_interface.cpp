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
    //float result = vertical + horizonttal + right horizontal
    if (remote.keyPressed(Remote::Key::D) && remote.keyPressed(Remote::Key::Q)){
        float trans = limitVal(0.1f, -1.0f, 1.0f);
        float rotate = limitVal(-0.1f, -1.0f, 1.0f);
        return trans + rotate; 
    }
    else-if (remote.keyPressed(Remote::Key::Q)){
        return limitVal(-0.1f, -1.0f, 1.0f);
    }
    else if  (remote.keyPressed(Remote::Key::W)){
        return limitVal(0.1f, -1.0f, 1.0f);
    }
    else if (remote.keyPressed(Remote::Key::A)){
        return limitVal(-0.1f, -1.0f, 1.0f);
    } 
    else if (remote.keyPressed(Remote::Key::S)){
        return limitVal(-0.1f, -1.0f, 1.0f);
    }
    else if (remote.keyPressed(Remote::Key::D)){
        return limitVal(0.1f, -1.0f, 1.0f);
    }
    else if (remote.keyPressed(Remote::Key::E)){
        return limitVal(0.1f, -1.0f, 1.0f);
    }
    else if (remote.keyPressed(Remote::Key::Q)){
        return limitVal(-0.1f, -1.0f, 1.0f);
    }
    else{
        return 0.0f;
    }
}

float ControlOperatorInterface::getChassisOmniLeftBackInput() {
    //float result = vertical - horizontal + rightHorizontal;
    if (remote.keyPressed(Remote::Key::D) && remote.keyPressed(Remote::Key::Q)){
        float trans = limitVal(-0.1f, -1.0f, 1.0f);
        float rotate = limitVal(-0.1f, -1.0f, 1.0f);
        return trans + rotate; 
    }
    else if  (remote.keyPressed(Remote::Key::W)){
        return limitVal(0.1f, -1.0f, 1.0f);
    }
    else if (remote.keyPressed(Remote::Key::A)){
        return limitVal(0.1f, -1.0f, 1.0f);
    } 
    else if (remote.keyPressed(Remote::Key::S)){
        return limitVal(-0.1f, -1.0f, 1.0f);
    }
    else if (remote.keyPressed(Remote::Key::D)){
        return limitVal(-0.1f, -1.0f, 1.0f);
    }
    else if (remote.keyPressed(Remote::Key::E)){
        return limitVal(0.1f, -1.0f, 1.0f);
    }
    else if (remote.keyPressed(Remote::Key::Q)){
        return limitVal(-0.1f, -1.0f, 1.0f);
    }
    else{
        return 0.0f;
    }
}

float ControlOperatorInterface::getChassisOmniRightFrontInput() {
    //float result = vertical - horizontal - rightHorizontal;
    if (remote.keyPressed(Remote::Key::D) && remote.keyPressed(Remote::Key::Q)){
        float trans = limitVal(-0.1f, -1.0f, 1.0f);
        float rotate = limitVal(0.1f, -1.0f, 1.0f);
        return trans + rotate; 
    }
    else if  (remote.keyPressed(Remote::Key::W)){
        return limitVal(0.1f, -1.0f, 1.0f);
    }
    else if (remote.keyPressed(Remote::Key::A)){
        return limitVal(0.1f, -1.0f, 1.0f);
    } 
    else if (remote.keyPressed(Remote::Key::S)){
        return limitVal(-0.1f, -1.0f, 1.0f);
    }
    else if (remote.keyPressed(Remote::Key::D)){
        return limitVal(-0.1f, -1.0f, 1.0f);
    }
    else if (remote.keyPressed(Remote::Key::E)){
        return limitVal(-0.1f, -1.0f, 1.0f);
    }
    else if (remote.keyPressed(Remote::Key::Q)){
        return limitVal(0.1f, -1.0f, 1.0f);
    }
    else{
        return 0.0f;
    }
}

float ControlOperatorInterface::getChassisOmniRightBackInput() {
    //float result = vertical + horizontal - rightHorizontal;
    if (remote.keyPressed(Remote::Key::D) && remote.keyPressed(Remote::Key::Q)){
        float trans = limitVal(0.1f, -1.0f, 1.0f);
        float rotate = limitVal(0.1f, -1.0f, 1.0f);
        return trans + rotate; 
    }
    else if  (remote.keyPressed(Remote::Key::W)){
        return limitVal(0.1f, -1.0f, 1.0f);
    }
    else if (remote.keyPressed(Remote::Key::A)){
        return limitVal(-0.1f, -1.0f, 1.0f);
    } 
    else if (remote.keyPressed(Remote::Key::S)){
        return limitVal(-0.1f, -1.0f, 1.0f);
    }
    else if (remote.keyPressed(Remote::Key::D)){
        return limitVal(0.1f, -1.0f, 1.0f);
    }
    else if (remote.keyPressed(Remote::Key::E)){
        return limitVal(-0.1f, -1.0f, 1.0f);
    }
    else if (remote.keyPressed(Remote::Key::Q)){
        return limitVal(0.1f, -1.0f, 1.0f);
    }
    else{
        return 0.0f;
    }
}

}  // namespace control 
