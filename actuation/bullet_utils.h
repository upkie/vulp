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

/*! Convert an Eigen quaternion to a Bullet one.
 *
 * \param[in] quat Eigen quaternion.
 *
 * \return Same quaternion for Bullet.
 */
inline btQuaternion bullet_from_eigen(const Eigen::Quaterniond& quat) {
  return btQuaternion(quat.x(), quat.y(), quat.z(), quat.w());
}

/*! Convert an Eigen vector to a Bullet one.
 *
 * \param[in] v Eigen vector.
 *
 * \return Same vector for Bullet.
 */
inline btVector3 bullet_from_eigen(const Eigen::Vector3d& v) {
  return btVector3(v.x(), v.y(), v.z());
}

/*! Convert a Bullet quaternion to an Eigen one.
 *
 * \param[in] quat Bullet quaternion.
 *
 * \return Same vector for Eigen.
 */
inline Eigen::Quaterniond eigen_from_bullet(const btQuaternion& quat) {
  return Eigen::Quaterniond(quat.getW(), quat.getX(), quat.getY(), quat.getZ());
}

/*! Convert a Bullet vector to an Eigen one.
 *
 * \param[in] quat Bullet vector.
 *
 * \return Same vector for Eigen.
 */
inline Eigen::Vector3d eigen_from_bullet(const btVector3& v) {
  return Eigen::Vector3d(v.getX(), v.getY(), v.getZ());
}

/*! Find the index of a link.
 *
 * \param[in] bullet Bullet client.
 * \param[in] robot Index of the robot to search.
 * \param[in] link_name Name of the searched link.
 *
 * \return Link index if found, -1 otherwise.
 */
inline int find_link_index(b3RobotSimulatorClientAPI& bullet, int robot,
                           const std::string& link_name) noexcept {
  b3JointInfo joint_info;
  int nb_joints = bullet.getNumJoints(robot);
  for (int joint_index = 0; joint_index < nb_joints; ++joint_index) {
    bullet.getJointInfo(robot, joint_index, &joint_info);
    if (std::string(joint_info.m_linkName) == link_name) {
      return joint_index;  // link and joint indices are the same in Bullet
    }
  }
  return -1;
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

  Eigen::Quaterniond orientation_imu_in_world;
  orientation_imu_in_world.x() = link_state.m_worldLinkFrameOrientation[0];
  orientation_imu_in_world.y() = link_state.m_worldLinkFrameOrientation[1];
  orientation_imu_in_world.z() = link_state.m_worldLinkFrameOrientation[2];
  orientation_imu_in_world.w() = link_state.m_worldLinkFrameOrientation[3];

  // The attitude reference system frame has +x forward, +y right and +z down,
  // whereas our world frame has +x forward, +y left and +z up:
  // https://github.com/mjbots/pi3hat/blob/master/docs/reference.md#orientation
  Eigen::Matrix3d rotation_world_to_ars =
      Eigen::Vector3d{1.0, -1.0, -1.0}.asDiagonal();

  Eigen::Matrix3d rotation_imu_to_world =
      orientation_imu_in_world.toRotationMatrix();
  Eigen::Matrix3d rotation_imu_to_ars =
      rotation_world_to_ars * rotation_imu_to_world;
  Eigen::Quaterniond orientation_imu_in_ars(rotation_imu_to_ars);

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

  Eigen::Matrix3d rotation_world_to_imu =
      orientation_imu_in_world.normalized().inverse();
  Eigen::Vector3d angular_velocity_imu_in_imu =
      rotation_world_to_imu * angular_velocity_imu_to_world_in_world;
  Eigen::Vector3d linear_acceleration_imu_in_imu =
      rotation_world_to_imu * linear_acceleration_imu_in_world;

  // Fill out regular IMU data
  imu_data.orientation_imu_in_ars = orientation_imu_in_ars;
  imu_data.angular_velocity_imu_in_imu = angular_velocity_imu_in_imu;
  imu_data.linear_acceleration_imu_in_imu = linear_acceleration_imu_in_imu;

  // ... and the extra field for the Bullet interface
  imu_data.linear_velocity_imu_in_world = linear_velocity_imu_in_world;
}

}  // namespace vulp::actuation
