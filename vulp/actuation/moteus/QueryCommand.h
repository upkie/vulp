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
 *     License: Apache-2.0 (see licenses/LICENSE-pi3hat)
 */

#pragma once

#include "vulp/actuation/moteus/Resolution.h"

namespace vulp::actuation::moteus {

struct QueryCommand {
  Resolution mode = Resolution::kInt16;
  Resolution position = Resolution::kInt16;
  Resolution velocity = Resolution::kInt16;
  Resolution torque = Resolution::kInt16;
  Resolution q_current = Resolution::kIgnore;
  Resolution d_current = Resolution::kIgnore;
  Resolution rezero_state = Resolution::kIgnore;
  Resolution voltage = Resolution::kInt8;
  Resolution temperature = Resolution::kInt8;
  Resolution fault = Resolution::kInt8;

  bool any_set() const {
    return mode != Resolution::kIgnore || position != Resolution::kIgnore ||
           velocity != Resolution::kIgnore || torque != Resolution::kIgnore ||
           q_current != Resolution::kIgnore ||
           d_current != Resolution::kIgnore ||
           rezero_state != Resolution::kIgnore ||
           voltage != Resolution::kIgnore ||
           temperature != Resolution::kIgnore || fault != Resolution::kIgnore;
  }
};

}  // namespace vulp::actuation::moteus
