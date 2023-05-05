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
#include "vulp/actuation/tests/coffee_machine_layout.h"

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
    const auto layout = get_coffee_machine_layout();
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
  ASSERT_EQ(interface_->commands().size(), get_coffee_machine_layout().size());
}

TEST_F(InterfaceTest, RepliesAreInitialized) {
  ASSERT_EQ(interface_->replies().size(), get_coffee_machine_layout().size());
}

TEST_F(InterfaceTest, DataIsInitialized) {
  ASSERT_EQ(interface_->data().commands.data(), interface_->commands().data());
  ASSERT_EQ(interface_->data().replies.data(), interface_->replies().data());
}

TEST_F(InterfaceTest, WriteStopCommands) {
  for (auto& command : interface_->commands()) {
    command.mode = actuation::moteus::Mode::kFault;
  }
  interface_->write_stop_commands();
  for (const auto& command : interface_->commands()) {
    ASSERT_EQ(command.mode, actuation::moteus::Mode::kStopped);
  }
}

}  // namespace vulp::actuation
