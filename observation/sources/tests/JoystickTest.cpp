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

#include <linux/joystick.h>

#include <fstream>

#include "gtest/gtest.h"
#include "vulp/observation/sources/Joystick.h"

namespace vulp::observation::sources {

TEST(Joystick, WriteOnce) {
  Joystick joystick;
  Dictionary observation;
  ASSERT_NO_THROW(joystick.write(observation));
}

TEST(Joystick, UnknownEvent) {
  std::ofstream file("jsX", std::ios::binary | std::ios::out);
  Joystick joystick("jsX");

  struct js_event event;
  event.type = JS_EVENT_INIT;
  file.write(reinterpret_cast<char*>(&event), sizeof(event));
  file.flush();

  Dictionary observation;
  ASSERT_NO_THROW(joystick.write(observation));
}

}  // namespace vulp::observation::sources
