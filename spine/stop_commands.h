/*
 * Copyright 2022 Stéphane Caron
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
