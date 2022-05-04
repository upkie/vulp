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

#include <Eigen/Geometry>
#include <limits>
#include <map>
#include <string>

#include "vulp/actuation/ImuData.h"
#include "vulp/actuation/Interface.h"
#include "vulp/actuation/moteus/protocol.h"

namespace vulp::actuation {

class MockInterface : public Interface {
 public:
  /*! Create mock actuator interface.
   *
   * \param[in] params Interface parameters.
   */
  MockInterface(const ServoLayout& layout, const double dt);

  //! Default destructor
  ~MockInterface() = default;

  /*! Reset interface.
   *
   * \param[in] config Additional configuration dictionary.
   */
  void reset(const palimpsest::Dictionary& config) override;

  /*! Simulate a new communication cycle.
   *
   * \param data Buffer to read commands from and write replies to.
   * \param callback Function to call when the cycle is over.
   *
   * The callback will be invoked from an arbitrary thread when the
   * communication cycle has completed. All memory pointed to by @p data must
   * remain valid until the callback is invoked.
   */
  void cycle(const moteus::Data& data,
             std::function<void(const moteus::Output&)> callback) final;

  //! Orientation from the IMU frame to the world frame.
  Eigen::Quaterniond get_attitude() const noexcept {
    return imu_data_.orientation_imu_in_world;
  }

  //! Body angular velocity of the IMU frame in [rad] / [s].
  Eigen::Vector3d get_angular_velocity() const noexcept {
    return imu_data_.angular_velocity_imu_in_imu;
  }

  //! Linear acceleration of the IMU, in [m] / [s]².
  Eigen::Vector3d get_linear_acceleration() const noexcept {
    return imu_data_.linear_acceleration_imu_in_imu;
  }

  //! Get the IMU attitude
  ImuData imu_data() const noexcept final { return imu_data_; }

 private:
  //! Spine timestep in [s].
  const double dt_;

  //! Mock servo replies, laid out as a servo_id -> query_result map
  std::map<int, moteus::QueryResult> query_results_;

  //! Mock IMU data
  ImuData imu_data_;
};

}  // namespace vulp::actuation
