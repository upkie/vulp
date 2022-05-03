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
 *
 * This file incorporates work covered by the following copyright and
 * permission notice:
 *
 *     Copyright 2020 Josh Pieper, jjp@pobox.com.
 *     License: Apache-2.0
 */

#pragma once

#include <limits>

#include "vulp/actuation/moteus/Mode.h"

namespace vulp::actuation::moteus {

struct QueryResult {
  //! Control mode
  Mode mode = Mode::kStopped;

  //! Angular position in [rev]
  double position = std::numeric_limits<double>::quiet_NaN();

  //! Angular velocity in [rps]
  double velocity = std::numeric_limits<double>::quiet_NaN();

  //! Torque in [Nm]
  double torque = std::numeric_limits<double>::quiet_NaN();

  //! Q-phase current in [A]
  double q_current = std::numeric_limits<double>::quiet_NaN();

  //! D-phase current in [A]
  double d_current = std::numeric_limits<double>::quiet_NaN();

  bool rezero_state = false;
  double voltage = std::numeric_limits<double>::quiet_NaN();
  double temperature = std::numeric_limits<double>::quiet_NaN();
  int fault = 0;
};

}  // namespace vulp::actuation::moteus
