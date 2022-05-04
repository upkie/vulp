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

}  // namespace vulp::actuation
