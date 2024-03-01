// Copyright 2022 Stéphane Caron
// SPDX-License-Identifier: Apache-2.0
/*
 * This file incorporates work covered by the following copyright and
 * permission notice:
 *
 *     Copyright 2020 Josh Pieper, jjp@pobox.com.
 *     SPDX-License-Identifier: Apache-2.0
 */

#pragma once

namespace vulp::actuation::moteus {

/*! Control mode
 *
 * This class mirrors the BldcServoMode in ``bldc_servo_structs.h``. See the
 * mjbots/moteus repository for details.
 */
enum class Mode {
  // In this mode, the entire motor driver will be disabled.
  //
  // When exiting this state, the current offset will be
  // recalibrated.
  kStopped = 0,

  // This stage cannot be commanded directly, but will be entered
  // upon any fault. Here, the motor driver remains enabled, but
  // the output stage power is removed. The only valid transition
  // from this state is to kStopped.
  kFault = 1,

  // This mode may not be commanded directly. It is used when
  // transitioning from kStopped to another mode.
  kEnabling = 2,

  // This mode may not be commanded directly, but is used when
  // transitioning from kStopped to another mode.
  kCalibrating = 3,

  // This mode may not be commanded directly, but is used when
  // transitioning from kStopped to another mode.
  kCalibrationComplete = 4,

  // Directly control the PWM of all 3 phases.
  kPwm = 5,

  // Control the voltage of all three phases
  kVoltage = 6,

  // Control the phase and voltage magnitude
  kVoltageFoc = 7,

  // Control d and q voltage
  kVoltageDq = 8,

  // Control d and q current
  kCurrent = 9,

  // Control absolute position
  kPosition = 10,

  // This state can be commanded directly, and will also be entered
  // automatically upon a watchdog timeout from kPosition.  When in
  // this state, the controller will apply the selected fallback
  // control mode.
  //
  // The only way to exit this state is through a stop command.
  kPositionTimeout = 11,

  // Control to zero velocity through a derivative only version of
  // the position mode.
  kZeroVelocity = 12,

  // This applies the PID controller only to stay within a
  // particular position region, and applies 0 torque when within
  // that region.
  kStayWithinBounds = 13,

  // This mode applies a fixed voltage square waveform in the D axis
  // in order to measure inductance assuming a motor with
  // approximately equal D/Q axis inductances.
  kMeasureInductance = 14,

  // All phases are pulled to ground.
  kBrake = 15,

  kPositionContinue = 42,  // Vulp-specific, not from moteus lib
};

/*! Name of a mode.
 *
 * \param[in] mode Mode to name.
 */
constexpr const char* to_string(const Mode& mode) noexcept {
  switch (mode) {
    case Mode::kStopped:
      return "Mode::kStopped";
    case Mode::kFault:
      return "Mode::kFault";
    case Mode::kEnabling:
      return "Mode::kEnabling";
    case Mode::kCalibrating:
      return "Mode::kCalibrating";
    case Mode::kCalibrationComplete:
      return "Mode::kCalibrationComplete";
    case Mode::kPwm:
      return "Mode::kPwm";
    case Mode::kVoltage:
      return "Mode::kVoltage";
    case Mode::kVoltageFoc:
      return "Mode::kVoltageFoc";
    case Mode::kVoltageDq:
      return "Mode::kVoltageDq";
    case Mode::kCurrent:
      return "Mode::kCurrent";
    case Mode::kPosition:
      return "Mode::kPosition";
    case Mode::kPositionTimeout:
      return "Mode::kPositionTimeout";
    case Mode::kZeroVelocity:
      return "Mode::kZeroVelocity";
    case Mode::kStayWithinBounds:
      return "Mode::kStayWithinBounds";
    case Mode::kMeasureInductance:
      return "Mode::kMeasureInductance";
    case Mode::kBrake:
      return "Mode::kBrake";
    case Mode::kPositionContinue:
      return "Mode::kPositionContinue";
    default:
      break;
  }
  return "?";
}

}  // namespace vulp::actuation::moteus
