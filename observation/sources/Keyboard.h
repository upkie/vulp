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

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <string>
#include <termios.h>
#include <string.h>
#include <iostream>
#include <chrono>

#include "vulp/observation/Source.h"

#define MAX_KEY_BYTES 3 // Maximum number of bytes to encode a key
#define KEY_POLLING_INTERVAL_MS 100 // Polling interval in milliseconds

// Byte sequences that encode arrow keys are platform specific
#ifdef __APPLE__
#define _HEAD 0xEF, 0x9C
#define UP_BYTES {_HEAD, 0x80}
#define DOWN_BYTES {_HEAD, 0x81}
#define RIGHT_BYTES {_HEAD, 0x83}
#define LEFT_BYTES {_HEAD, 0x82}
#else
#define _HEAD 0x1B, 0x5B
#define UP_BYTES {_HEAD, 0x41}
#define DOWN_BYTES {_HEAD, 0x42}
#define RIGHT_BYTES {_HEAD, 0x43}
#define LEFT_BYTES {_HEAD, 0x44}
#endif

#define is_lowercase_alpha(c) 0x61 <= c && c <= 0x7A // Checks if 'a' <= c <= 'z'
#define is_uppercase_alpha(c) 0x41 <= c && c <= 0x5A // Checks if 'A' <= c <= 'Z'
#define is_printable_ascii(c) 0x20 <= c && c <= 0x7F

using namespace std::chrono;

namespace vulp::observation::sources {

/*! Source for a Keyboard controller.
 * *
 * \note This source reads from the standard input.
 */
class Keyboard : public Source {
 public:
  /*! Open the device file.
   *
   * \note Use \ref present to check if the joystick was opened successfully.
   */
  Keyboard();

  //! Close device file.
  ~Keyboard() override;

  enum key {
    // Directional keys (arrows)
    UP = 0,
    DOWN = 1,
    LEFT = 2,
    RIGHT = 3,
    // Single char keys (alphabetical)
    W = 87,
    A = 65,
    S = 83,
    D = 68,
    X = 88,
    // Unknown key
    UNKNOWN = 255 // Map everything else to this key
  };

  //! Prefix of output in the observation dictionary.
  inline std::string prefix() const noexcept final { return "keyboard"; }

  /*! Write output to a dictionary.
   *
   * \param[out] output Dictionary to write observations to.
   */
  void write(Dictionary& output) final;

 private:
  //! Read next joystick event from the device file.
  int read_event();
  
  key map_char_to_key(unsigned char * buf);

  unsigned char buf_[MAX_KEY_BYTES]; // Read ASCII and non-ASCII characters (e.g. arrows)

  key key_code_; // Key code of the last key pressed
  bool key_pressed_; // Whether the last key pressed is still pressed

  // Last time a key was pressed in milliseconds
  ::system_clock::time_point last_key_poll_time_;

};

}  // namespace vulp::observation::sources
