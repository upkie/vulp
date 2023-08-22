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

#include "vulp/utils/random_string.h"

#include <string>

#include "gtest/gtest.h"

namespace vulp::utils {

TEST(RandomString, HasCorrectLength) {
  ASSERT_EQ(random_string(16).size(), 16);
  ASSERT_EQ(random_string(32).size(), 32);
}

TEST(RandomString, TrimmedAtSixtyTwo) {
  ASSERT_EQ(random_string(62).size(), 62);
  ASSERT_EQ(random_string(63).size(), 62);
  ASSERT_EQ(random_string(123456).size(), 62);
}

TEST(RandomString, DoesNotRepeat) {
  std::string first_string = random_string();
  std::string second_string = random_string();
  ASSERT_NE(first_string, second_string);
}

}  // namespace vulp::utils
