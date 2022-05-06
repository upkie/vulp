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

#include "vulp/observation/sources/Joystick.h"

namespace vulp::observation::sources {

Joystick::Joystick() {
  fd_ = ::open("/dev/input/js0", O_RDONLY | O_NONBLOCK);
  if (fd_ < 0) {
    spdlog::warn("Joystick observation disabled: no joystick found");
  }
}

Joystick::~Joystick() {
  if (fd_ >= 0) {
    ::close(fd_);
  }
}

void Joystick::read_event() {
  ssize_t bytes = ::read(fd_, &event_, sizeof(event_));
  if (bytes != sizeof(event_)) {  // no input
    return;
  }

  double normalized_value = static_cast<double>(event_.value) / 32767;
  if (std::abs(normalized_value) < kJoystickDeadband) {
    normalized_value = 0.0;
  }

  switch (event_.type) {
    case JS_EVENT_BUTTON:
      switch (event_.number) {
        case 0:  // PS4: cross, Xbox: A
          cross_button_ = event_.value;
          break;
        case 1:  // PS4: circle, Xbox: B
          throw std::runtime_error(event_.value ? "Stop button pressed"
                                                : "Stop button released");
          break;
        case 2:  // PS4: triangle, Xbox: X
          triangle_button_ = event_.value;
          break;
        case 3:  // PS4: square, Xbox: Y
          square_button_ = event_.value;
          break;
        case 4:  // PS4: L1, Xbox: L
          left_button_ = event_.value;
          break;
        case 5:  // PS4: R1, Xbox: R
          right_button_ = event_.value;
          break;
        case 6:  // PS4: L2, Xbox: back
          break;
        case 7:  // PS4: R2, Xbox: start
          break;
        case 8:  // PS4: share, Xbox: Xbox button
          break;
        case 9:  // PS4: options, Xbox: left stick button
          break;
        case 10:  // PS4: PS, Xbox: right stick button
          break;
        case 11:  // PS4: L3, Xbox: N/A
          break;
        case 12:  // PS4: R3, Xbox: N/A
          break;
        default:
          spdlog::warn("Button number {} is out of range", event_.number);
          break;
      }
      break;
    case JS_EVENT_AXIS:
      switch (event_.number) {
        case 0:
          left_axis_.x() = normalized_value;
          break;
        case 1:
          left_axis_.y() = normalized_value;
          break;
        case 2:
          left_trigger_ = normalized_value;
          break;
        case 3:
          right_axis_.x() = normalized_value;
          break;
        case 4:
          right_axis_.y() = normalized_value;
          break;
        case 5:
          right_trigger_ = normalized_value;
          break;
        case 6:
          pad_axis_.x() = normalized_value;
          break;
        case 7:
          pad_axis_.y() = normalized_value;
          break;
        default:
          spdlog::warn("Axis number {} is out of range", event_.number);
          break;
      }
      break;
    default:
      // initialization events
      break;
  }
}

void Joystick::write(Dictionary& observation) {
  read_event();
  auto& output = observation(prefix());
  output("cross_button") = cross_button_;
  output("left_axis") = left_axis_;
  output("left_button") = left_button_;
  output("left_trigger") = left_trigger_;
  output("pad_axis") = pad_axis_;
  output("right_axis") = right_axis_;
  output("right_button") = right_button_;
  output("right_trigger") = right_trigger_;
  output("square_button") = square_button_;
  output("triangle_button") = triangle_button_;
}

}  // namespace vulp::observation::sources
