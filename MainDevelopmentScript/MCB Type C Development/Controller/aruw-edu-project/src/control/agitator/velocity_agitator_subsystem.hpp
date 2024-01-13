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

#pragma once

#include "tap/algorithms/smooth_pid.hpp"
#include "tap/control/setpoint/interfaces/integrable_setpoint_subsystem.hpp"
#include "tap/motor/dji_motor.hpp"

#include "control/algorithms/edu_pid.hpp"

class Drivers;

namespace control::agitator
{
/**
 * Subsystem whose primary purpose is to encapsulate an agitator motor that operates using a
 * velocity controller. Also keeps track of absolute position to allow commands to rotate the
 * agitator some specific displacement.
 */
class VelocityAgitatorSubsystem : public tap::control::setpoint::IntegrableSetpointSubsystem
{
public:
    /**
     * Agitator gear ratios of different motors, for determining shaft rotation angle.
     */
    static constexpr float AGITATOR_GEAR_RATIO_M2006 = 36.0f;

    /**
     * Construct an agitator with the passed in velocity PID parameters, gear ratio, and
     * agitator-specific configuration.
     *
     * @param[in] drivers Reference to aruwsrc drivers struct
     * @param[in] pidConfig PID configuration struct for the agitator motor controller.
     * @param[in] agitator The base motor that this agitator subsystem is is going to control.
     */
    VelocityAgitatorSubsystem(
        Drivers& drivers,
        const control::algorithms::EduPidConfig& pidConfig,
        tap::motor::DjiMotor& agitator);

    void initialize() override;

    /**
     * @brief Checks if the agitator is connected and runs the velocity PID controller.
     */
    void refresh() override;

    const char* getName() override { return "velocity agitator"; }

    /// @return The velocity setpoint that some command has requested, in radians / second
    float getSetpoint() const override;

    /**
     * Sets the velocity setpoint to the specified velocity
     *
     * @param[in] velocity The desired velocity in radians / second.
     */
    void setSetpoint(float velocity) override { velocitySetpoint = velocity; }

    /// @return The agitator velocity in radians / second.
    float getCurrentValue() const override;

    /**
     * Meaningless function that nothing uses
     * @return 0
     */
    inline float getJamSetpointTolerance() const override { return 0; }

    /**
     * Attempts to calibrate the agitator at the current position, such that `getPosition` will
     * return 0 radians at this position.
     *
     * @return `true` if the agitator has been successfully calibrated, `false` otherwise.
     */
    bool calibrateHere() override;

    /**
     * @brief Because this agitator doesn't support jam detection and unjamming, this function
     * always returns false.
     */
    bool isJammed() override { return false; }

    /**
     * Because this agitator doesn't support unjamming, this function does nothing.
     */
    void clearJam() override {}

    /**
     * @return `true` if the agitator has been calibrated (`calibrateHere` has been called and the
     * agitator motor is online).
     */
    bool isCalibrated() override { return calibrated; }

    /**
     * @return `true` if the agitator motor is online (i.e.: is connected)
     */
    bool isOnline() override;

    /**
     * Since we don't keep track of the derivative of the velocity (since the velocity is the
     * setpoint), this function will always return 0.
     *
     * @return 0
     */
    inline float getVelocity() override { return 0; }

    /**
     * @return The calibrated agitator angle, in radians (integral of velocity is radians). If the
     * agitator is uncalibrated, 0 radians is returned.
     */
    float getCurrentValueIntegral() const override;

private:
    /// @return Uncalibrated agitator angle.
    float getUncalibratedAgitatorAngle() const;

    tap::motor::DjiMotor& agitator;

    control::algorithms::EduPid velocityPid;

    float velocitySetpoint{0};

    bool calibrated{false};

    float agitatorCalibratedZeroAngle{0};

    uint32_t prevTime{0};
};

}  // namespace control::agitator
