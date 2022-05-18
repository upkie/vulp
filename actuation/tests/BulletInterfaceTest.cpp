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
#include "tools/cpp/runfiles/runfiles.h"
#include "vulp/actuation/BulletInterface.h"

namespace vulp::actuation {

using bazel::tools::cpp::runfiles::Runfiles;

class BulletInterfaceTest : public ::testing::Test {
 protected:
  //! Set up a new test fixture
  void SetUp() override {
    Dictionary config;
    config("bullet")("gui") = false;
    config("spine_frequency") = 1000u;

    ServoLayout layout;
    layout.add_servo(1, 1, "right_hip");
    layout.add_servo(2, 1, "right_knee");
    layout.add_servo(3, 1, "right_wheel");
    layout.add_servo(4, 2, "left_hip");
    layout.add_servo(5, 2, "left_knee");
    layout.add_servo(6, 2, "left_wheel");

    std::string error;
    std::unique_ptr<Runfiles> runfiles(Runfiles::CreateForTest(&error));
    ASSERT_NE(runfiles, nullptr);

    BulletInterface::Parameters params(config);
    params.urdf_path = runfiles->Rlocation("upkie_description/urdf/upkie.urdf");
    interface_ = std::make_unique<BulletInterface>(layout, params);

    for (const auto& pair : layout.servo_joint_map()) {
      commands_.push_back({});
      commands_.back().id = pair.first;
    }
    replies_.resize(commands_.size());
    data_.commands = {commands_.data(), commands_.size()};
    data_.replies = {replies_.data(), replies_.size()};
  }

 protected:
  //! Bullet actuation interface
  std::unique_ptr<BulletInterface> interface_;

  //! Servo commands placeholder
  std::vector<actuation::moteus::ServoCommand> commands_;

  //! Servo replies placeholder
  std::vector<actuation::moteus::ServoReply> replies_;

  //! Data buffer to call interface_->cycle()
  moteus::Data data_;
};

TEST_F(BulletInterfaceTest, CycleCallsCallback) {
  moteus::Data data;
  bool callback_called = false;
  interface_->cycle(data, [&callback_called](const moteus::Output& output) {
    callback_called = true;
  });
  ASSERT_TRUE(callback_called);
}

TEST_F(BulletInterfaceTest, MaxTorquesFromURDF) {
  const auto& max_torque = interface_->max_torque();
  ASSERT_NO_THROW(max_torque.at("left_hip"));
  ASSERT_NO_THROW(max_torque.at("left_knee"));
  ASSERT_NO_THROW(max_torque.at("left_wheel"));
  ASSERT_NO_THROW(max_torque.at("right_hip"));
  ASSERT_NO_THROW(max_torque.at("right_knee"));
  ASSERT_NO_THROW(max_torque.at("right_wheel"));
  ASSERT_GT(max_torque.at("left_hip"), 5.0);
  ASSERT_GT(max_torque.at("left_knee"), 5.0);
  ASSERT_GT(max_torque.at("left_wheel"), 0.5);
  ASSERT_GT(max_torque.at("right_hip"), 5.0);
  ASSERT_GT(max_torque.at("right_knee"), 5.0);
  ASSERT_GT(max_torque.at("right_wheel"), 0.5);
}

TEST_F(BulletInterfaceTest, CycleDoesntThrow) {
  ASSERT_NO_THROW(
      interface_->cycle(data_, [](const moteus::Output& output) {}));
}

}  // namespace vulp::actuation
