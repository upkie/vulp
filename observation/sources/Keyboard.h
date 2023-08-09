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

#pragma once

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>

#include <chrono>
#include <iostream>
#include <string>

#include "vulp/observation/Source.h"

#define MAX_KEY_BYTES 3              // Maximum number of bytes to encode a key
#define KEY_POLLING_INTERVAL_MS 100  // Polling interval in milliseconds

// Byte sequences that encode arrow keys are platform specific
#ifndef __APPLE__
const unsigned char UP_BYTES[] = {0xEF, 0x9C, 0x80};
const unsigned char DOWN_BYTES[] = {0xEF, 0x9C, 0x81};
const unsigned char RIGHT_BYTES[] = {0xEF, 0x9C, 0x83};
const unsigned char LEFT_BYTES[] = {0xEF, 0x9C, 0x82};
#else
const unsigned char UP_BYTES[] = {0x1B, 0x5B, 0x41};
const unsigned char DOWN_BYTES[] = {0x1B, 0x5B, 0x42};
const unsigned char RIGHT_BYTES[] = {0x1B, 0x5B, 0x43};
const unsigned char LEFT_BYTES[] = {0x1B, 0x5B, 0x44};
#endif

#define is_lowercase_alpha(c) \
  0x61 <= c&& c <= 0x7A  // Checks if 'a' <= c <= 'z'
#define is_uppercase_alpha(c) \
  0x41 <= c&& c <= 0x5A  // Checks if 'A' <= c <= 'Z'
#define is_printable_ascii(c) 0x20 <= c&& c <= 0x7F

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::system_clock;

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
    UNKNOWN = 255  // Map everything else to this key
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

  key map_char_to_key(unsigned char* buf);

  unsigned char
      buf_[MAX_KEY_BYTES];  // Read ASCII and non-ASCII characters (e.g. arrows)

  key key_code_;            // Key code of the last key pressed
  bool key_pressed_;        // Whether the last key pressed is still pressed

  // Last time a key was pressed in milliseconds
  system_clock::time_point last_key_poll_time_;
};

}  // namespace vulp::observation::sources
