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

#include "vulp/actuation/moteus/Resolution.h"

namespace vulp::actuation::moteus {

struct PositionResolution {
  Resolution position = Resolution::kFloat;
  Resolution velocity = Resolution::kFloat;
  Resolution feedforward_torque = Resolution::kFloat;
  Resolution kp_scale = Resolution::kFloat;
  Resolution kd_scale = Resolution::kFloat;
  Resolution maximum_torque = Resolution::kIgnore;
  Resolution stop_position = Resolution::kFloat;
  Resolution watchdog_timeout = Resolution::kFloat;
};

}  // namespace vulp::actuation::moteus
