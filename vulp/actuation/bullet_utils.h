// SPDX-License-Identifier: Apache-2.0
// Copyright 2022 Stéphane Caron

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

}  // namespace vulp::actuation
