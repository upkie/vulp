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
    params.dt = 1.0 / 1000.0;
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

TEST_F(BulletInterfaceTest, JointProperties) {
  const auto& joint_props = interface_->joint_properties();

  ASSERT_NO_THROW(joint_props.at("left_hip"));
  ASSERT_NO_THROW(joint_props.at("left_knee"));
  ASSERT_NO_THROW(joint_props.at("left_wheel"));
  ASSERT_NO_THROW(joint_props.at("right_hip"));
  ASSERT_NO_THROW(joint_props.at("right_knee"));
  ASSERT_NO_THROW(joint_props.at("right_wheel"));

  constexpr double kNoFriction = 1e-5;
  ASSERT_LT(joint_props.at("left_hip").friction, kNoFriction);
  ASSERT_LT(joint_props.at("left_knee").friction, kNoFriction);
  ASSERT_LT(joint_props.at("left_wheel").friction, kNoFriction);
  ASSERT_LT(joint_props.at("right_hip").friction, kNoFriction);
  ASSERT_LT(joint_props.at("right_knee").friction, kNoFriction);
  ASSERT_LT(joint_props.at("right_wheel").friction, kNoFriction);

  ASSERT_GT(joint_props.at("left_hip").maximum_torque, 5.0);
  ASSERT_GT(joint_props.at("left_knee").maximum_torque, 5.0);
  ASSERT_GT(joint_props.at("left_wheel").maximum_torque, 0.5);
  ASSERT_GT(joint_props.at("right_hip").maximum_torque, 5.0);
  ASSERT_GT(joint_props.at("right_knee").maximum_torque, 5.0);
  ASSERT_GT(joint_props.at("right_wheel").maximum_torque, 0.5);
}

TEST_F(BulletInterfaceTest, CycleDoesntThrow) {
  ASSERT_NO_THROW(
      interface_->cycle(data_, [](const moteus::Output& output) {}));
}

TEST_F(BulletInterfaceTest, ResetBaseState) {
  Dictionary config;
  config("bullet")("gui") = false;
  config("bullet")("reset")("orientation_base_in_world") =
      Eigen::Quaterniond(0.707, 0.0, -0.707, 0.0);
  config("bullet")("reset")("position_base_in_world") =
      Eigen::Vector3d(0.0, 0.0, 1.0);
  config("bullet")("reset")("linear_velocity_base_to_world_in_world") =
      Eigen::Vector3d(4.0, 5.0, 6.0);
  config("bullet")("reset")("angular_velocity_base_in_base") =
      Eigen::Vector3d(7.0, 8.0, 9.0);
  interface_->reset(config);

  const Eigen::Matrix4d T = interface_->transform_base_to_world();
  const Eigen::Matrix3d R = T.block<3, 3>(0, 0);
  EXPECT_NEAR(R(0, 0), 0.0, 1e-7);
  EXPECT_NEAR(R(0, 2), -1.0, 1e-7);
  EXPECT_NEAR(R(1, 1), 1.0, 1e-7);
  EXPECT_NEAR(R(2, 0), 1.0, 1e-7);
  EXPECT_NEAR(R(2, 2), 0.0, 1e-7);

  const Eigen::Vector3d p = T.block<3, 1>(0, 3);
  ASSERT_DOUBLE_EQ(p.x(), 0.0);
  ASSERT_DOUBLE_EQ(p.y(), 0.0);
  ASSERT_DOUBLE_EQ(p.z(), 1.0);

  const Eigen::Vector3d v =
      interface_->linear_velocity_base_to_world_in_world();
  ASSERT_DOUBLE_EQ(v.x(), 4.0);
  ASSERT_DOUBLE_EQ(v.y(), 5.0);
  ASSERT_DOUBLE_EQ(v.z(), 6.0);

  const Eigen::Vector3d omega = interface_->angular_velocity_base_in_base();
  ASSERT_NEAR(omega.x(), 7.0, 1e-3);
  ASSERT_NEAR(omega.y(), 8.0, 1e-3);
  ASSERT_NEAR(omega.z(), 9.0, 7e-3);
}

}  // namespace vulp::actuation
