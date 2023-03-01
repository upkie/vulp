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

#include <palimpsest/Dictionary.h>

#include <Eigen/Geometry>
#include <map>
#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "vulp/observation/ObserverError.h"

namespace vulp::observation::tests {

TEST(ObserverError, NineteenSixtyWho) {
  const std::string prefix = "nineteen_sixty";
  const std::string key = "what";
  ObserverError error(prefix, key);
  const std::string msg = error.what();
  ASSERT_NE(msg.find(prefix), std::string::npos);
  ASSERT_NE(msg.find(key), std::string::npos);
}

}  // namespace vulp::observation::tests
