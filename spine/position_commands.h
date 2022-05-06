/*
 * Copyright 2022 Stéphane Caron
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <palimpsest/Dictionary.h>

#include <limits>
#include <map>
#include <string>
#include <vector>

#include "vulp/actuation/moteus/Mode.h"
#include "vulp/actuation/moteus/ServoCommand.h"

namespace vulp::spine {

namespace defaults {

constexpr double kKdScale = 1.0;

constexpr double kKpScale = 1.0;

// TODO(scaron): this is a sane default for a qdd100, not for weaker servos
constexpr double kMaximumTorque = 8.42;  // N.m

constexpr double kVelocity = 0.0;  // rad/s

}  // namespace defaults

/*! Initialize action dictionary with the keys expected by \ref
 * write_position_commands.
 *
 * \param[out] action Action dictionary.
 * \param[in] servo_joint_map Map from servo ID to joint name.
 */
inline void expect_position_commands(
    palimpsest::Dictionary& action,
    const std::map<int, std::string>& servo_joint_map) {
  for (const auto& id_joint : servo_joint_map) {
    const std::string& joint_name = id_joint.second;
    auto& servo_action = action("servo")(joint_name);
    servo_action("position") = std::numeric_limits<double>::quiet_NaN();
    servo_action("velocity") = 0.0;
    servo_action("kp_scale") = defaults::kKpScale;
    servo_action("kd_scale") = defaults::kKdScale;
    servo_action("maximum_torque") = defaults::kMaximumTorque;
  }
}

/*! Write position commands for all servos.
 *
 * \param[out] commands Servo commands to set to stop.
 * \param[in] servo_joint_map Map from servo ID to joint name.
 * \param[in] action Action to read commands from.
 */
inline void write_position_commands(
    std::vector<actuation::moteus::ServoCommand>& commands,
    const std::map<int, std::string>& servo_joint_map,
    const palimpsest::Dictionary& action) {
  using Mode = actuation::moteus::Mode;
  if (!action.has("servo")) {
    spdlog::warn("No position command at key \"servo\" of action");
    return;
  }
  const auto& servo = action("servo");
  for (auto& command : commands) {
    const int servo_id = command.id;
    auto it = servo_joint_map.find(servo_id);
    if (it == servo_joint_map.end()) {
      spdlog::error("Unknown servo ID {} in CAN command", servo_id);
      command.mode = Mode::kStopped;
      continue;
    }
    const auto& joint = it->second;
    if (!servo.has(joint)) {
      spdlog::error("No action for joint {} (servo ID={})", joint, servo_id);
      command.mode = Mode::kStopped;
      continue;
    }
    const auto& servo_action = servo(joint);
    if (!servo_action.has("position")) {
      spdlog::error("No position command for joint {} (servo ID={})", joint,
                    servo_id);
      command.mode = Mode::kStopped;
      continue;
    }

    const double position_rad = servo_action("position");
    const double velocity_rad_s =
        servo_action.get<double>("velocity", defaults::kVelocity);
    const double kp_scale =
        servo_action.get<double>("kp_scale", defaults::kKpScale);
    const double kd_scale =
        servo_action.get<double>("kd_scale", defaults::kKdScale);
    const double maximum_torque =
        servo_action.get<double>("maximum_torque", defaults::kMaximumTorque);

    // The moteus convention is that positive angles correspond to clockwise
    // rotations when looking at the rotor / back of the moteus board. See:
    // https://jpieper.com/2021/04/30/moteus-direction-configuration/
    const double position_rev = position_rad / (2.0 * M_PI);
    const double velocity_rev_s = velocity_rad_s / (2.0 * M_PI);

    command.mode = Mode::kPosition;
    command.position.position = position_rev;
    command.position.velocity = velocity_rev_s;
    command.position.kp_scale = kp_scale;
    command.position.kd_scale = kd_scale;
    command.position.maximum_torque = maximum_torque;
  }
}

}  // namespace vulp::spine
