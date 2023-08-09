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

#include "vulp/observation/sources/Keyboard.h"

namespace vulp::observation::sources {
Keyboard::Keyboard() {
  // Constructor
  termios term;
  tcgetattr(STDIN_FILENO, &term);
  term.c_lflag &= ~ICANON;
  tcsetattr(STDIN_FILENO, TCSANOW, &term);
  setbuf(stdin, NULL);
}

Keyboard::~Keyboard() {
  // Destructor
}

int Keyboard::read_event() {
    // Check if there are bytes to be read from the STDIN
    ssize_t bytes = 0;
    ioctl(STDIN_FILENO, FIONREAD, &bytes);
    
    // Read if that's the case!
    if(bytes){
        long read_bytes = ::read(STDIN_FILENO, &buf_, bytes);
        
        if(read_bytes != bytes){
            throw std::runtime_error("All bytes could not be read from the standard input!");
        }
        
        return 1;
    }
    
    return 0;
}

Keyboard::key Keyboard::map_char_to_key(unsigned char *buf){
    // Check for 3-byte characters (i.e. arrows)
    if(!memcmp(buf_, (unsigned char[])DOWN_BYTES, MAX_KEY_BYTES)){
        return key::DOWN;
    }
    if(!memcmp(buf_, (unsigned char[])UP_BYTES, MAX_KEY_BYTES)){
        return key::UP;
    }
    if(!memcmp(buf_, (unsigned char[])LEFT_BYTES, MAX_KEY_BYTES)){
        return key::LEFT;
    }
    if(!memcmp(buf_, (unsigned char[])RIGHT_BYTES, MAX_KEY_BYTES)){
        return key::RIGHT;
    }
    
    // If the first byte corresponds to a lowercase ASCII alphabetic
    if(is_lowercase_alpha(buf[0])){
        buf[0] -= 32; // Map to uppercase equivalent;
    }
    
    // We treat any printable ASCII as a single key code
    if(is_printable_ascii(buf[0])){
        switch (buf[0])
        {
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
  bool key_pressed = read_event();
  key key_code = map_char_to_key(buf_);

  auto& output = observation(prefix());
  output("key_pressed") = key_pressed;
  output("up_arrow") = key_code == key::UP;
  output("down_arrow") = key_code == key::DOWN;
  output("left_arrow") = key_code == key::LEFT;
  output("right_arrow") = key_code == key::RIGHT;
  output("W") = key_code == key::W;
  output("A") = key_code == key::A;
  output("S") = key_code == key::S;
  output("D") = key_code == key::D;
  output("X") = key_code == key::X;
  output("unknown_key") = key_code == key::UNKNOWN;

}

}  // namespace vulp::observation::sources
