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

#include "vulp/observation/observe_servos.h"

#include <map>
#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "vulp/actuation/moteus/ServoReply.h"

namespace moteus = vulp::actuation::moteus;

using palimpsest::Dictionary;

namespace vulp::observation::tests {

TEST(Servo, ReadTorques) {
  Dictionary observation;

  std::map<int, std::string> servo_joint_map = {{0, "foo"}, {1, "bar"}};
  std::vector<moteus::ServoReply> servo_replies;
  servo_replies.push_back({1, {}});           // bar first
  servo_replies.back().result.torque = -10.;  // N.m
  servo_replies.push_back({0, {}});           // foo next
  servo_replies.back().result.torque = 10.;   // N.m

  observe_servos(observation, servo_joint_map, servo_replies);

  ASSERT_TRUE(observation.has("servo"));
  ASSERT_TRUE(observation("servo").has("foo"));
  ASSERT_TRUE(observation("servo").has("bar"));
  ASSERT_DOUBLE_EQ(observation("servo")("foo")("torque"), 10.);
  ASSERT_DOUBLE_EQ(observation("servo")("bar")("torque"), -10.);
}

}  // namespace vulp::observation::tests
