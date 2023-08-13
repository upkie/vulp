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
  termios term;
  tcgetattr(STDIN_FILENO, &term);
  term.c_lflag &= ~ICANON;
  tcsetattr(STDIN_FILENO, TCSANOW, &term);
  setbuf(stdin, NULL);

  key_pressed_ = false;
  key_code_ = Key::UNKNOWN;

  last_key_poll_time_ = system_clock::now() - milliseconds(kPollingIntervalMS);
}

Keyboard::~Keyboard() {}

Key Keyboard::read_event() {
  ssize_t bytes_available = 0;
  ioctl(STDIN_FILENO, FIONREAD, &bytes_available);

  if (bytes_available < 1) {
    return Key::NONE;
  }

  Key key_code = Key::UNKNOWN;

  int bytes_to_read = std::min(bytes_available, (ssize_t)kMaxReadBytes);

  // Read bytes from STDIN, shifting the buffer left by one byte each time
  for(int i = 0; i < bytes_to_read + kMaxKeyBytes; i++) {
    shift_left(buf_, kMaxKeyBytes);

    if(i < bytes_available){
      int bytes_read = ::read(STDIN_FILENO, &buf_[kMaxKeyBytes - 1], 1);

      if (bytes_read < 1) {
        spdlog::warn("Bytes could not be read from stdin!");
      }
    }

    key_code = map_char_to_key(buf_);

    /* Return UNKNOWN only if we have read all available bytes
     * and shifted the last byte read to the first position without
     * finding a match.
     */
    if (key_code != Key::UNKNOWN)
      return key_code;
    
  } // for
  return key_code; // UNKNOWN
}

Key Keyboard::map_char_to_key(unsigned char* buf) {
  // Check for 3-byte characters first (i.e. arrows)
  if (!memcmp(buf_, DOWN_BYTES, kMaxKeyBytes)) {
    return Key::DOWN;
  }
  if (!memcmp(buf_, UP_BYTES, kMaxKeyBytes)) {
    return Key::UP;
  }
  if (!memcmp(buf_, LEFT_BYTES, kMaxKeyBytes)) {
    return Key::LEFT;
  }
  if (!memcmp(buf_, RIGHT_BYTES, kMaxKeyBytes)) {
    return Key::RIGHT;
  }

  // If the first byte corresponds to a lowercase ASCII alphabetic
  if (is_lowercase_alpha(buf[0])) {
    buf[0] -= 32;  // Map to uppercase equivalent
  }

  // We treat any printable ASCII as a single key code
  if (is_printable_ascii(buf[0])) {
    switch (buf[0]) {
      case 87:
        return Key::W;
      case 65:
        return Key::A;
      case 83:
        return Key::S;
      case 68:
        return Key::D;
      case 88:
        return Key::X;
    }
  }
  return Key::UNKNOWN;
}

void Keyboard::shift_left(unsigned char* arr, size_t array_size = kMaxKeyBytes) {
  for (int i = 0; i < array_size - 1; i++) {
    arr[i] = arr[i + 1];
  }
  arr[array_size - 1] = 0;
}

void Keyboard::write(Dictionary& observation) {
  // Check elapsed time since last key polling
  auto elapsed = system_clock::now() - last_key_poll_time_;
  auto elapsed_ms = duration_cast<milliseconds>(elapsed).count();

  // Poll for key press if enough time has elapsed or if no key is pressed
  if (elapsed_ms >= kPollingIntervalMS || !key_pressed_) {
    key_code_ = read_event();
    key_pressed_ = key_code_ != Key::NONE;
    printf("Key pressed: %d\n", key_pressed_);
    printf("Key code: %02x\n", key_code_);
  }

  auto& output = observation(prefix());
  output("key_pressed") = key_pressed_;
  output("up") = key_code_ == Key::UP;
  output("down") = key_code_ == Key::DOWN;
  output("left") = key_code_ == Key::LEFT;
  output("right") = key_code_ == Key::RIGHT;
  output("w") = key_code_ == Key::W;
  output("a") = key_code_ == Key::A;
  output("s") = key_code_ == Key::S;
  output("d") = key_code_ == Key::D;
  output("x") = key_code_ == Key::X;
}

}  // namespace vulp::observation::sources
