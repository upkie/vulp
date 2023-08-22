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

#include "vulp/actuation/MockInterface.h"

using std::chrono::duration_cast;
using std::chrono::microseconds;

namespace vulp::actuation {

MockInterface::MockInterface(const ServoLayout& layout, const double dt)
    : Interface(layout), dt_(dt) {
  query_results_.clear();
  for (const auto& id_bus : layout.servo_bus_map()) {
    const auto& servo_id = id_bus.first;
    moteus::QueryResult result;
    result.d_current = std::numeric_limits<double>::quiet_NaN();
    result.fault = 0;
    result.q_current = std::numeric_limits<double>::quiet_NaN();
    result.rezero_state = false;
    result.temperature = std::numeric_limits<double>::quiet_NaN();
    result.voltage = std::numeric_limits<double>::quiet_NaN();
    query_results_.try_emplace(servo_id, result);
  }
}

void MockInterface::reset(const palimpsest::Dictionary& config) {}

void MockInterface::cycle(const moteus::Data& data,
                          std::function<void(const moteus::Output&)> callback) {
  assert(data.replies.size() == data.commands.size());

  for (const auto& command : data.commands) {
    const auto servo_id = command.id;
    auto& result = query_results_[servo_id];
    result.mode = command.mode;
    const auto& target = command.position;
    if (std::isnan(target.position) && !std::isnan(target.velocity) &&
        !std::isnan(result.position)) {
      result.position += target.velocity * dt_;
    } else {
      result.position = target.position;
    }
    result.velocity = target.velocity;
    result.torque = target.feedforward_torque;
  }

  moteus::Output output;
  for (size_t i = 0; i < data.replies.size(); ++i) {
    const auto servo_id = data.commands[i].id;
    data.replies[i].id = servo_id;
    data.replies[i].result = query_results_[servo_id];
    output.query_result_size = i + 1;
  }
  callback(output);
}

}  // namespace vulp::actuation