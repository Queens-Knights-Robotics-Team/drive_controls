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
    if  (remote.keyPressed(Remote::Key::W)){
        float vertical   = limitVal((Remote::Key::W), 1.0f, -1.0f, 1.0f);
        float result = vertical;
        return result;
    }
    else if (remote.keyPressed(Remote::Key::A)){
        float horizontal = limitVal((Remote::Key::A),-1.0f, -1.0f, 1.0f);
        float result = horizontal;
        return result;
    } 
    else if (remote.keyPressed(Remote::Key::S)){
        float vertical   = limitVal((Remote::Key::S), -1.0f, -1.0f, 1.0f);
        float result = vertical;
        return result;
    }
    else if (remote.keyPressed(Remote::Key::D)){
        float horizontal = limitVal((Remote::Key::D),1.0f, -1.0f, 1.0f);
        float result = horizontal;
        return result;
    }
    else{
        float vertical = limitVal(0.0f, 1.0f,1.0f);
        float result = vertical;
        return result
    }
}

float ControlOperatorInterface::getChassisOmniLeftBackInput() {
    //float result = vertical - horizontal + rightHorizontal;
    if  (remote.keyPressed(Remote::Key::W)){
        float vertical   = limitVal((Remote::Key::W), 1.0f, -1.0f, 1.0f);
        float result = vertical;
        return result;
    }
    else if (remote.keyPressed(Remote::Key::A)){
        float horizontal = limitVal((Remote::Key::A),-1.0f, -1.0f, 1.0f);
        float result = horizontal;
        return result;
    } 
    else if (remote.keyPressed(Remote::Key::S)){
        float vertical   = limitVal((Remote::Key::S), -1.0f, -1.0f, 1.0f);
        float result = vertical;
        return result;
    }
    else if (remote.keyPressed(Remote::Key::D)){
        float horizontal = limitVal((Remote::Key::D),1.0f, -1.0f, 1.0f);
        float result = horizontal;
        return result;
    }
    else{
        float vertical = limitVal(0.0f, 1.0f,1.0f);
        float result = vertical;
        return result
    }
}

float ControlOperatorInterface::getChassisOmniRightFrontInput() {
    //float result = vertical - horizontal - rightHorizontal;

    if  (remote.keyPressed(Remote::Key::W)){
        float vertical   = limitVal((Remote::Key::W), 1.0f, -1.0f, 1.0f);
        float result = vertical;
        return result;
    }
    else if (remote.keyPressed(Remote::Key::A)){
        float horizontal = limitVal((Remote::Key::A),-1.0f, -1.0f, 1.0f);
        float result = horizontal;
        return result;
    } 
    else if (remote.keyPressed(Remote::Key::S)){
        float vertical   = limitVal((Remote::Key::S), -1.0f, -1.0f, 1.0f);
        float result = vertical;
        return result;
    }
    else if (remote.keyPressed(Remote::Key::D)){
        float horizontal = limitVal((Remote::Key::D),1.0f, -1.0f, 1.0f);
        float result = horizontal;
        return result;
    }
    else{
        float vertical = limitVal(0.0f, 1.0f,1.0f);
        float result = vertical;
        return result
    }
}

float ControlOperatorInterface::getChassisOmniRightBackInput() {
    //float result = vertical + horizontal - rightHorizontal;
    if  (remote.keyPressed(Remote::Key::W)){
        float vertical   = limitVal((Remote::Key::W), 1.0f, -1.0f, 1.0f);
        float result = vertical;
        return result;
    }
    else if (remote.keyPressed(Remote::Key::A)){
        float horizontal = limitVal((Remote::Key::A),-1.0f, -1.0f, 1.0f);
        float result = horizontal;
        return result;
    } 
    else if (remote.keyPressed(Remote::Key::S)){
        float vertical   = limitVal((Remote::Key::S), -1.0f, -1.0f, 1.0f);
        float result = vertical;
        return result;
    }
    else if (remote.keyPressed(Remote::Key::D)){
        float horizontal = limitVal((Remote::Key::D),1.0f, -1.0f, 1.0f);
        float result = horizontal;
        return result;
    }
    else{
        float vertical = limitVal(0.0f, 1.0f,1.0f);
        float result = vertical;
        return result
    }
}

}  // namespace control 
