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

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "vulp/actuation/Interface.h"

namespace vulp::actuation {

class SmallInterface : public Interface {
 public:
  using Interface::Interface;

  void reset(const palimpsest::Dictionary& config) override {}

  void cycle(const moteus::Data& data,
             std::function<void(const moteus::Output&)> callback) final {}

  ImuData imu_data() const noexcept final { return ImuData(); }
};

class InterfaceTest : public ::testing::Test {
 protected:
  //! Set up a new test fixture
  void SetUp() override {
    ServoLayout layout;
    layout.add_servo(1, 1, "right_mill");
    layout.add_servo(2, 1, "right_grinder");
    layout.add_servo(3, 1, "right_pump");
    layout.add_servo(4, 2, "left_mill");
    layout.add_servo(5, 2, "left_grinder");
    layout.add_servo(6, 2, "left_pump");

    interface_ = std::make_unique<SmallInterface>(layout);
  }

 protected:
  //! Actuation interface
  std::unique_ptr<Interface> interface_;

  //! Servo commands placeholder
  std::vector<actuation::moteus::ServoCommand> commands_;

  //! Servo replies placeholder
  std::vector<actuation::moteus::ServoReply> replies_;

  //! Data buffer to call interface_->cycle()
  moteus::Data data_;
};

TEST_F(InterfaceTest, CommandsAreInitialized) {
  ASSERT_EQ(interface_->commands().size(), 6);
}

TEST_F(InterfaceTest, RepliesAreInitialized) {
  ASSERT_EQ(interface_->replies().size(), 6);
}

TEST_F(InterfaceTest, DataIsInitialized) {
  ASSERT_EQ(interface_->data().commands.data(), interface_->commands().data());
  ASSERT_EQ(interface_->data().replies.data(), interface_->replies().data());
}

}  // namespace vulp::actuation
