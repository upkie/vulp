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

TEST(Joystick, ReadButtons) {
  std::ofstream file("jsX", std::ios::binary | std::ios::out);
  Joystick joystick("jsX");

  struct js_event event;
  event.type = JS_EVENT_BUTTON;
  event.number = 0;
  event.value = 1;
  file.write(reinterpret_cast<char*>(&event), sizeof(event));
  file.flush();

  Dictionary observation;
  joystick.write(observation);
  const auto& output = observation(joystick.prefix());
  ASSERT_TRUE(output.get<bool>("cross_button"));
  ASSERT_FALSE(output.get<bool>("square_button"));
  ASSERT_FALSE(output.get<bool>("triangle_button"));

  for (uint8_t button = 2; button < 14; button++) {
    event.number = button;
    file.write(reinterpret_cast<char*>(&event), sizeof(event));
    file.flush();
    joystick.write(observation);
    ASSERT_EQ(output.get<bool>("right_button"), (button >= 5));
  }
}

TEST(Joystick, EmergencyStop) {
  std::ofstream file("jsX", std::ios::binary | std::ios::out);
  Joystick joystick("jsX");

  struct js_event event;
  event.type = JS_EVENT_BUTTON;
  event.number = 1;
  event.value = 1;
  file.write(reinterpret_cast<char*>(&event), sizeof(event));
  file.flush();

  Dictionary observation;
  ASSERT_THROW(joystick.write(observation), std::runtime_error);
}

}  // namespace vulp::observation::sources
