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

#include "vulp/spine/position_commands.h"

#include <map>
#include <string>
#include <vector>

#include "gtest/gtest.h"

namespace vulp::spine {

using palimpsest::Dictionary;

class PositionCommandsTest : public ::testing::Test {
 protected:
  void SetUp() override {
    servo_joint_map_.insert({0, "foo"});
    servo_joint_map_.insert({1, "bar"});
  }

  std::map<int, std::string> servo_joint_map_;
};

TEST_F(PositionCommandsTest, ExpectFillsDictionaryKeys) {
  Dictionary action;
  expect_position_commands(action, servo_joint_map_);
  ASSERT_TRUE(action.has("servo"));
  const Dictionary& servo = action("servo");
  for (const auto joint_name : {"foo", "bar"}) {
    ASSERT_TRUE(servo.has(joint_name));
    ASSERT_TRUE(servo(joint_name).has("position"));
    ASSERT_TRUE(servo(joint_name).has("velocity"));
    ASSERT_TRUE(servo(joint_name).has("kp_scale"));
    ASSERT_TRUE(servo(joint_name).has("kd_scale"));
    ASSERT_TRUE(servo(joint_name).has("maximum_torque"));
  }
}

TEST_F(PositionCommandsTest, DontThrowIfNoServoAction) {
  std::vector<actuation::moteus::ServoCommand> commands;
  Dictionary action;
  ASSERT_NO_THROW(write_position_commands(commands, servo_joint_map_, action));
}

TEST_F(PositionCommandsTest, StopUnknownServos) {
  std::vector<actuation::moteus::ServoCommand> commands;
  commands.push_back({});
  commands.back().id = 0;
  commands.push_back({});
  commands.back().id = 2;  // not in servo_joint_map_
  Dictionary action;
  action("servo")("foo")("position") = 0.0;
  write_position_commands(commands, servo_joint_map_, action);
  ASSERT_EQ(commands[0].mode, actuation::moteus::Mode::kPosition);
  ASSERT_EQ(commands[1].mode, actuation::moteus::Mode::kStopped);
}

TEST_F(PositionCommandsTest, ForwardPositionCommands) {
  std::vector<actuation::moteus::ServoCommand> commands;
  commands.push_back({});
  commands.back().id = 0;
  commands.push_back({});
  commands.back().id = 1;

  Dictionary action;
  action("servo")("foo")("position") = 2 * M_PI;  // [rad]
  action("servo")("bar")("position") = M_PI;      // [rad]

  write_position_commands(commands, servo_joint_map_, action);
  for (const auto& command : commands) {
    ASSERT_EQ(command.mode, actuation::moteus::Mode::kPosition);
  }
  ASSERT_DOUBLE_EQ(commands[0].position.position, 1.0);  // [rev]
  ASSERT_DOUBLE_EQ(commands[1].position.position, 0.5);  // [rev]
}

TEST_F(PositionCommandsTest, StopServoIfNoAction) {
  std::vector<actuation::moteus::ServoCommand> commands;
  commands.push_back({});
  commands.back().id = 1;
  Dictionary action;
  action("servo")("foo")("position") = 2 * M_PI;  // not "bar"
  write_position_commands(commands, servo_joint_map_, action);
  ASSERT_EQ(commands[0].mode, actuation::moteus::Mode::kStopped);
}

TEST_F(PositionCommandsTest, StopServoIfNoPosition) {
  std::vector<actuation::moteus::ServoCommand> commands;
  commands.push_back({});
  commands.back().id = 0;
  Dictionary action;
  action("servo")("foo")("velocity") = 1.5;  // [rad] / [s]
  write_position_commands(commands, servo_joint_map_, action);
  ASSERT_EQ(commands[0].mode, actuation::moteus::Mode::kStopped);
}

TEST_F(PositionCommandsTest, ForwardVelocityCommands) {
  std::vector<actuation::moteus::ServoCommand> commands;
  commands.push_back({});
  commands.back().id = 0;
  commands.push_back({});
  commands.back().id = 1;

  Dictionary action;
  action("servo")("foo")("position") = 2 * M_PI;  // [rad]
  action("servo")("bar")("position") = M_PI;      // [rad]
  action("servo")("foo")("velocity") = M_PI / 2;  // [rad]
  action("servo")("bar")("velocity") = M_PI / 2;  // [rad]

  write_position_commands(commands, servo_joint_map_, action);
  for (const auto& command : commands) {
    ASSERT_EQ(command.mode, actuation::moteus::Mode::kPosition);
  }
  ASSERT_DOUBLE_EQ(commands[0].position.position, 1.0);   // [rev]
  ASSERT_DOUBLE_EQ(commands[1].position.position, 0.5);   // [rev]
  ASSERT_DOUBLE_EQ(commands[0].position.velocity, 0.25);  // [rps]
  ASSERT_DOUBLE_EQ(commands[1].position.velocity, 0.25);  // [rps]
}

}  // namespace vulp::spine
