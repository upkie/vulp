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
  char *tmpfn = tmpnam(nullptr);
  std::ofstream tmpf(tmpfn);
  tmpf << "A"; //(unsigned char[]){'A'};
  tmpf.close();

  freopen(tmpfn, "r", stdin);

  Keyboard keyboard = Keyboard();

  Dictionary observation;
  keyboard.write(observation);

  const auto& output = observation(keyboard.prefix());
  ASSERT_TRUE(output.get<bool>("key_pressed"));
  ASSERT_TRUE(output.get<bool>("A"));
  ASSERT_FALSE(output.get<bool>("W"));
  ASSERT_FALSE(output.get<bool>("S"));
  ASSERT_FALSE(output.get<bool>("D"));
  ASSERT_FALSE(output.get<bool>("X"));
}

TEST(Keyboard, ReadArrows) {
  // We cannot write directly to STDIN, so we'll redirect a file to it
  char *tmpfn = tmpnam(nullptr);
  std::ofstream tmpf(tmpfn);
  tmpf << LEFT_BYTES;
  tmpf.close();

  freopen(tmpfn, "r", stdin);

  Keyboard keyboard = Keyboard();

  Dictionary observation;
  keyboard.write(observation);

  const auto& output = observation(keyboard.prefix());
  ASSERT_TRUE(output.get<bool>("key_pressed"));
  ASSERT_TRUE(output.get<bool>("LEFT"));
  ASSERT_FALSE(output.get<bool>("UP")); 
  ASSERT_FALSE(output.get<bool>("DOWN"));
  ASSERT_FALSE(output.get<bool>("RIGHT"));
  ASSERT_FALSE(output.get<bool>("W"));
  ASSERT_FALSE(output.get<bool>("A"));
  ASSERT_FALSE(output.get<bool>("S"));
  ASSERT_FALSE(output.get<bool>("D"));
  ASSERT_FALSE(output.get<bool>("X"));
}

TEST(Keyboard, ReadEmptySTDIN) {
  Keyboard keyboard = Keyboard();

  Dictionary observation;
  keyboard.write(observation);

  const auto& output = observation(keyboard.prefix());
  ASSERT_FALSE(output.get<bool>("key_pressed"));
  ASSERT_FALSE(output.get<bool>("LEFT"));
  ASSERT_FALSE(output.get<bool>("UP")); 
  ASSERT_FALSE(output.get<bool>("DOWN"));
  ASSERT_FALSE(output.get<bool>("RIGHT"));
  ASSERT_FALSE(output.get<bool>("W"));
  ASSERT_FALSE(output.get<bool>("A"));
  ASSERT_FALSE(output.get<bool>("S"));
  ASSERT_FALSE(output.get<bool>("D"));
  ASSERT_FALSE(output.get<bool>("X"));
}

}  // namespace vulp::observation::sources
