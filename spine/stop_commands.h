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

#include <vector>

#include "vulp/actuation/moteus/Mode.h"
#include "vulp/actuation/moteus/ServoCommand.h"

namespace vulp::spine {

/*! Stop all servos.
 *
 * \param[out] commands Servo commands to set to stop.
 *
 * This function does not and should not throw, as it will be called by default
 * if any exception is caught from the spine control loop.
 */
inline void write_stop_commands(
    std::vector<actuation::moteus::ServoCommand>& commands) noexcept {
  for (auto& command : commands) {
    command.mode = actuation::moteus::Mode::kStopped;
  }
}

}  // namespace vulp::spine
