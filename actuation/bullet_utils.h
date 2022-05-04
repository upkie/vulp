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

#pragma once

#include <spdlog/spdlog.h>

#include <string>

#include "RobotSimulator/b3RobotSimulatorClientAPI.h"
#include "vulp/actuation/BulletImuData.h"

namespace vulp::actuation {

/*! Convert an Eigen vector to a Bullet one.
 *
 * \param[in] v Eigen vector.
 * \return Same vector for Bullet.
 */
inline btVector3 bullet_from_eigen(const Eigen::Vector3d& v) {
  return btVector3(v.x(), v.y(), v.z());
}

/*! Convert a Bullet quaternion to an Eigen one.
 *
 * \param[in] quat Bullet quaternion.
 * \return Same vector for Eigen.
 */
inline Eigen::Quaterniond eigen_from_bullet(const btQuaternion& quat) {
  return Eigen::Quaterniond(quat.getW(), quat.getX(), quat.getY(), quat.getZ());
}

/*! Convert a Bullet vector to an Eigen one.
 *
 * \param[in] quat Bullet vector.
 * \return Same vector for Eigen.
 */
inline Eigen::Vector3d eigen_from_bullet(const btVector3& v) {
  return Eigen::Vector3d(v.getX(), v.getY(), v.getZ());
}

/*! Compute IMU readings from the IMU link state.
 *
 * \param[out] imu_data IMU data to update.
 * \param[in] bullet Bullet client.
 * \param[in] robot Bullet index of the robot model.
 * \param[in] imu_link_index Index of the IMU link in the robot.
 * \param[in] dt Simulation timestep in [s].
 */
inline void read_imu_data(BulletImuData& imu_data,
                          b3RobotSimulatorClientAPI& bullet, int robot,
                          const int imu_link_index, double dt) {
  b3LinkState link_state;
  bullet.getLinkState(robot, imu_link_index, /* computeVelocity = */ true,
                      /* computeForwardKinematics = */ true, &link_state);
  imu_data.orientation_imu_in_world.x() =
      link_state.m_worldLinkFrameOrientation[0];
  imu_data.orientation_imu_in_world.y() =
      link_state.m_worldLinkFrameOrientation[1];
  imu_data.orientation_imu_in_world.z() =
      link_state.m_worldLinkFrameOrientation[2];
  imu_data.orientation_imu_in_world.w() =
      link_state.m_worldLinkFrameOrientation[3];
  Eigen::Vector3d linear_velocity_imu_in_world = {
      link_state.m_worldLinearVelocity[0],
      link_state.m_worldLinearVelocity[1],
      link_state.m_worldLinearVelocity[2],
  };
  Eigen::Vector3d angular_velocity_imu_to_world_in_world = {
      link_state.m_worldAngularVelocity[0],
      link_state.m_worldAngularVelocity[1],
      link_state.m_worldAngularVelocity[2],
  };

  // Compute linear acceleration in the world frame by discrete differentiation
  const auto& previous_linear_velocity = imu_data.linear_velocity_imu_in_world;
  Eigen::Vector3d linear_acceleration_imu_in_world =
      (linear_velocity_imu_in_world - previous_linear_velocity) / dt;

  auto rotation_world_to_imu =
      imu_data.orientation_imu_in_world.normalized().inverse();
  imu_data.angular_velocity_imu_in_imu =
      rotation_world_to_imu * angular_velocity_imu_to_world_in_world;
  imu_data.linear_acceleration_imu_in_imu =
      rotation_world_to_imu * linear_acceleration_imu_in_world;
  imu_data.linear_velocity_imu_in_world = linear_velocity_imu_in_world;
}

}  // namespace vulp::actuation
