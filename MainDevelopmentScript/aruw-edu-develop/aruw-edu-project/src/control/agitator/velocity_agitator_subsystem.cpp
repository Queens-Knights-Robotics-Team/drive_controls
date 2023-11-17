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

#include "velocity_agitator_subsystem.hpp"

#include "tap/architecture/clock.hpp"

#include "modm/math/geometry/angle.hpp"

#include "drivers.hpp"

using tap::arch::clock::getTimeMilliseconds;
using tap::motor::DjiMotor;

namespace control::agitator
{
// STEP 1 (Agitator Control): create constructor

// STEP 2 (Agitator Control): initialize function

// STEP 6 (Agitator Control): refresh function

// STEP 3 (Agitator Control): getSetpoint function

// STEP 4 (Agitator Control): getCurrentValue function

// STEP 5 (Agitator Control): calibrateHere function

// STEP 7 (Agitator Control): isOnline function

// STEP 8 (Agitator Control): getCurrentValueIntegral function

float VelocityAgitatorSubsystem::getUncalibratedAgitatorAngle() const
{
    return (2.0f * M_PI / static_cast<float>(DjiMotor::ENC_RESOLUTION)) *
           agitator.getEncoderUnwrapped() / AGITATOR_GEAR_RATIO_M2006;
}
}  // namespace control::agitator
