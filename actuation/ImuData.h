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

#include <Eigen/Core>
#include <Eigen/Geometry>

namespace vulp::actuation {

//! Data filtered from an onboard IMU such as the pi3hat's.
struct ImuData {
  //! Orientation from the IMU frame to the world frame.
  Eigen::Quaterniond orientation_imu_in_ars = Eigen::Quaterniond::Identity();

  /*! Body angular velocity of the IMU frame in [rad] / [s].
   *
   * \note The full name of the body angular vector would be "angular velocity
   * IMU to world in IMU", but as for all body angular velocities, "angular
   * velocity Body to X in Body" is the same for all inertial frames X (that
   * have zero velocity relative to the world frame). See for instance
   * https://scaron.info/robotics/screw-theory.html#body-screws for the math.
   */
  Eigen::Vector3d angular_velocity_imu_in_imu = Eigen::Vector3d::Zero();

  //! Body linear acceleration of the IMU, in [m] / [s]².
  Eigen::Vector3d linear_acceleration_imu_in_imu = Eigen::Vector3d::Zero();
};

}  // namespace vulp::actuation
