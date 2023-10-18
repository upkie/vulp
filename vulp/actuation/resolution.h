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

#include "vulp/actuation/moteus/PositionResolution.h"
#include "vulp/actuation/moteus/QueryCommand.h"

namespace vulp::actuation {

/*! Query resolution settings for all servo commands.
 *
 * \return Query resolution settings.
 *
 * For now these settings are common to all interfaces but we can easily turn
 * them into parameters.
 */
inline actuation::moteus::QueryCommand get_query_resolution() {
  using actuation::moteus::Resolution;
  actuation::moteus::QueryCommand query;
  query.mode = Resolution::kInt16;
  query.position = Resolution::kInt32;
  query.velocity = Resolution::kInt32;
  query.torque = Resolution::kInt32;
  query.q_current = Resolution::kIgnore;
  query.d_current = Resolution::kIgnore;
  query.rezero_state = Resolution::kIgnore;
  query.voltage = Resolution::kInt8;
  query.temperature = Resolution::kInt8;
  query.fault = Resolution::kInt8;
  return query;
}

/*! Resolution settings for all servo commands.
 *
 * \return Resolution settings.
 *
 * For now these settings are common to all interfaces but we can easily turn
 * them into parameters.
 */
inline actuation::moteus::PositionResolution get_position_resolution() {
  using actuation::moteus::Resolution;
  actuation::moteus::PositionResolution resolution;
  resolution.position = Resolution::kFloat;
  resolution.velocity = Resolution::kFloat;
  resolution.feedforward_torque = Resolution::kIgnore;
  resolution.kp_scale = Resolution::kFloat;
  resolution.kd_scale = Resolution::kFloat;
  resolution.maximum_torque = Resolution::kFloat;
  resolution.stop_position = Resolution::kIgnore;
  resolution.watchdog_timeout = Resolution::kIgnore;
  return resolution;
}

}  // namespace vulp::actuation
