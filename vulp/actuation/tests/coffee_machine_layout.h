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

#include "vulp/actuation/ServoLayout.h"

namespace vulp::actuation {

/*! Get a sample servo layout.
 *
 * \return Sample servo layout.
 */
inline const ServoLayout get_coffee_machine_layout() noexcept {
  ServoLayout layout;
  layout.add_servo(1, 1, "left_grinder");
  layout.add_servo(3, 1, "left_pump");
  layout.add_servo(5, 2, "right_grinder");
  layout.add_servo(6, 2, "right_pump");
  return layout;
}

}  // namespace vulp::actuation
