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
#include <fstream>

#include "gtest/gtest.h"
#include "vulp/observation/sources/Keyboard.h"

namespace vulp::observation::sources {

TEST(Keyboard, WriteOnce) {
  Keyboard keyboard;
  Dictionary observation;
  ASSERT_NO_THROW(keyboard.write(observation));
}

TEST(Keyboard, ReadAlphabetical) {
  // We cannot write directly to STDIN, so we'll redirect a file to it
  char* tmpfn = tmpnam(nullptr);
  std::ofstream tmpf(tmpfn);
  tmpf << "A";  //(unsigned char[]){'A'};
  tmpf.close();

  freopen(tmpfn, "r", stdin);

  Keyboard keyboard = Keyboard();

  Dictionary observation;
  keyboard.write(observation);

  const auto& output = observation(keyboard.prefix());
  ASSERT_TRUE(output.get<bool>("key_pressed"));
  ASSERT_FALSE(output.get<bool>("left"));
  ASSERT_FALSE(output.get<bool>("up"));
  ASSERT_FALSE(output.get<bool>("down"));
  ASSERT_FALSE(output.get<bool>("right"));
  ASSERT_FALSE(output.get<bool>("w"));
  ASSERT_TRUE(output.get<bool>("a"));
  ASSERT_FALSE(output.get<bool>("s"));
  ASSERT_FALSE(output.get<bool>("d"));
  ASSERT_FALSE(output.get<bool>("x"));
}

TEST(Keyboard, ReadArrows) {
  // We cannot write directly to STDIN, so we'll redirect a file to it
  char* tmpfn = tmpnam(nullptr);
  std::ofstream tmpf(tmpfn);
  tmpf << LEFT_BYTES;
  tmpf.close();

  freopen(tmpfn, "r", stdin);

  Keyboard keyboard = Keyboard();

  Dictionary observation;
  keyboard.write(observation);

  const auto& output = observation(keyboard.prefix());
  ASSERT_TRUE(output.get<bool>("key_pressed"));
  ASSERT_TRUE(output.get<bool>("left"));
  ASSERT_FALSE(output.get<bool>("up"));
  ASSERT_FALSE(output.get<bool>("down"));
  ASSERT_FALSE(output.get<bool>("right"));
  ASSERT_FALSE(output.get<bool>("w"));
  ASSERT_FALSE(output.get<bool>("a"));
  ASSERT_FALSE(output.get<bool>("s"));
  ASSERT_FALSE(output.get<bool>("d"));
  ASSERT_FALSE(output.get<bool>("x"));
}

TEST(Keyboard, ReadEmptySTDIN) {
  fflush(stdin);  // Clear STDIN

  Keyboard keyboard = Keyboard();

  Dictionary observation;
  keyboard.write(observation);

  const auto& output = observation(keyboard.prefix());
  ASSERT_FALSE(output.get<bool>("key_pressed"));
  ASSERT_FALSE(output.get<bool>("left"));
  ASSERT_FALSE(output.get<bool>("up"));
  ASSERT_FALSE(output.get<bool>("down"));
  ASSERT_FALSE(output.get<bool>("right"));
  ASSERT_FALSE(output.get<bool>("w"));
  ASSERT_FALSE(output.get<bool>("a"));
  ASSERT_FALSE(output.get<bool>("s"));
  ASSERT_FALSE(output.get<bool>("d"));
  ASSERT_FALSE(output.get<bool>("x"));
}

}  // namespace vulp::observation::sources
