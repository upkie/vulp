/*
 * Copyright 2023 Inria
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

#include "vulp/observation/sources/Keyboard.h"

namespace vulp::observation::sources {
Keyboard::Keyboard() {
  // Constructor
  termios term;
  tcgetattr(STDIN_FILENO, &term);
  term.c_lflag &= ~ICANON;
  tcsetattr(STDIN_FILENO, TCSANOW, &term);
  setbuf(stdin, NULL);

  key_pressed_ = false;
  key_code_ = key::UNKNOWN;

  // Check current time in milliseconds
  last_key_poll_time_ =
      system_clock::now() - milliseconds(KEY_POLLING_INTERVAL_MS);
}

Keyboard::~Keyboard() {
  // Destructor
}

int Keyboard::read_event() {
  // Check if there are bytes to be read from the STDIN
  ssize_t bytes_available = 0;
  ioctl(STDIN_FILENO, FIONREAD, &bytes_available);

  // Read if that's the case!
  if (bytes_available) {
    int bytes_read = ::read(STDIN_FILENO, &buf_, MAX_KEY_BYTES);

    if (bytes_read != bytes_available) {
      spdlog::warn("All bytes could not be read from the standard input!");
    }

    // Flush the standard input buffer, in case there are stale commands
    ::fflush(stdin);

    return 1;
  }

  return 0;
}

Keyboard::key Keyboard::map_char_to_key(unsigned char* buf) {
  // Check for 3-byte characters (i.e. arrows)
  if (!memcmp(buf_, DOWN_BYTES, MAX_KEY_BYTES)) {
    return key::DOWN;
  }
  if (!memcmp(buf_, UP_BYTES, MAX_KEY_BYTES)) {
    return key::UP;
  }
  if (!memcmp(buf_, LEFT_BYTES, MAX_KEY_BYTES)) {
    return key::LEFT;
  }
  if (!memcmp(buf_, RIGHT_BYTES, MAX_KEY_BYTES)) {
    return key::RIGHT;
  }

  // If the first byte corresponds to a lowercase ASCII alphabetic
  if (is_lowercase_alpha(buf[0])) {
    buf[0] -= 32;  // Map to uppercase equivalent;
  }

  // We treat any printable ASCII as a single key code
  if (is_printable_ascii(buf[0])) {
    switch (buf[0]) {
      case key::W:
        return key::W;
      case key::A:
        return key::A;
      case key::S:
        return key::S;
      case key::D:
        return key::D;
      case key::X:
        return key::X;
    }
  }
  return key::UNKNOWN;
}

void Keyboard::write(Dictionary& observation) {
  // Check elapsed time since last key polling
  auto elapsed = system_clock::now() - last_key_poll_time_;
  auto elapsed_ms = duration_cast<milliseconds>(elapsed).count();

  if (elapsed_ms >= KEY_POLLING_INTERVAL_MS) {
    key_pressed_ = read_event();

    if (key_pressed_){
      key_code_ = map_char_to_key(buf_);
    }else{
      key_code_ = key::UNKNOWN;
    }

    if(key_pressed_ && key_code_ == key::UNKNOWN) {
      key_pressed_ = false;
    }
  }

  auto& output = observation(prefix());
  output("key_pressed") = key_pressed_;
  output("UP") = key_code_ == key::UP;
  output("DOWN") = key_code_ == key::DOWN;
  output("LEFT") = key_code_ == key::LEFT;
  output("RIGHT") = key_code_ == key::RIGHT;
  output("W") = key_code_ == key::W;
  output("A") = key_code_ == key::A;
  output("S") = key_code_ == key::S;
  output("D") = key_code_ == key::D;
  output("X") = key_code_ == key::X;
}

}  // namespace vulp::observation::sources
