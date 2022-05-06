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
 *
 * This file incorporates work covered by the following copyright and
 * permission notice:
 *
 *     Copyright 2020 Josh Pieper, jjp@pobox.com.
 *     License: Apache-2.0 (see licenses/LICENSE-pi3hat)
 */

#pragma once

#include <mjbots/pi3hat/pi3hat.h>
#include <pthread.h>
#include <spdlog/spdlog.h>

#include <Eigen/Geometry>
#include <condition_variable>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "vulp/actuation/ImuData.h"
#include "vulp/actuation/Interface.h"
#include "vulp/actuation/moteus/protocol.h"
#include "vulp/utils/realtime.h"

namespace vulp::actuation {

using Pi3Hat = ::mjbots::pi3hat::Pi3Hat;

/*! Interface to moteus controllers.
 *
 * Internally it uses a background thread to operate the pi3hat, enabling the
 * main thread to perform work while servo communication is taking place.
 */
class Pi3HatInterface : public Interface {
 public:
  /*! Configure interface and spawn CAN thread.
   *
   * \param[in] layout Servo layout.
   * \param[in] can_cpu CPUID of the core to run the CAN thread on.
   * \param[in] pi3hat_config Configuration for the pi3hat.
   */
  Pi3HatInterface(const ServoLayout& layout, const int can_cpu,
                  const Pi3Hat::Configuration& pi3hat_config);

  //! Stop CAN thread
  ~Pi3HatInterface();

  /*! Reset interface.
   *
   * \param[in] config Additional configuration dictionary.
   */
  void reset(const palimpsest::Dictionary& config) override;

  /*! Spin a new communication cycle.
   *
   * \param[in] data Buffer to read commands from and write replies to.
   * \param[in] callback Function to call when the cycle is over.
   *
   * The callback will be invoked from an arbitrary thread when the
   * communication cycle has completed. All memory pointed to by \p data must
   * remain valid until the callback is invoked.
   */
  void cycle(const moteus::Data& data,
             std::function<void(const moteus::Output&)> callback) final;

  /*! Get orientation from the IMU frame to the world frame.
   *
   * This orientation is computed by the Unscented Kalman filter in
   * pi3hat/fw/ukf_filter.h.
   */
  Eigen::Quaterniond get_attitude() const noexcept {
    const double w = attitude_.attitude.w;
    const double x = attitude_.attitude.x;
    const double y = attitude_.attitude.y;
    const double z = attitude_.attitude.z;
    // These values were floats so the resulting quaternion is only
    // approximately normalized. We saw this property in d7fcaa97fa.
    return Eigen::Quaterniond(w, x, y, z).normalized();
  }

  /*! Get the body angular velocity of the IMU frame in [rad] / [s].
   *
   * \note This is the angular velocity \f$ {}_B \omega_{WB} \f$ from the IMU
   * frame \f$ B \f$ to the world frame \f$ W \f$, expressed in the IMU frame.
   */
  Eigen::Vector3d get_angular_velocity() const noexcept {
    const double omega_x = attitude_.rate_dps.x * M_PI / 180.;
    const double omega_y = attitude_.rate_dps.y * M_PI / 180.;
    const double omega_z = attitude_.rate_dps.z * M_PI / 180.;
    return {omega_x, omega_y, omega_z};
  }

  /*! Get the body linear acceleration of the IMU, in [m] / [s]².
   *
   * \note This is the linear acceleration \f$ {}_B a \f$ of the IMU frame \f$
   * B \f$ with respect to the world frame, expressed in the IMU frame.
   */
  Eigen::Vector3d get_linear_acceleration() const noexcept {
    const double a_x = attitude_.accel_mps2.x;
    const double a_y = attitude_.accel_mps2.y;
    const double a_z = attitude_.accel_mps2.z;
    return {a_x, a_y, a_z};
  }

  //! Get the pi3hat's IMU attitude
  ImuData imu_data() const noexcept final {
    ImuData imu_data;
    imu_data.orientation_imu_in_world = get_attitude();
    imu_data.angular_velocity_imu_in_imu = get_angular_velocity();
    imu_data.linear_acceleration_imu_in_imu = get_linear_acceleration();
    return imu_data;
  }

 private:
  /*! Main loop of the CAN thread.
   *
   * Synchronizes with \ref cycle via the internal condition variable.
   */
  void run_can_thread();

  /*! Execute one communication cycle on the CAN bus.
   *
   * Also, request the latest filtered attitude from the pi3hat.
   */
  moteus::Output cycle_can_thread();

 private:
  //! CPUID of the core to run the CAN thread on.
  const int can_cpu_;

  // pi3hat configuration
  const Pi3Hat::Configuration pi3hat_config_;

  //! Mutex associated with \ref can_wait_condition_
  std::mutex mutex_;

  //! Condition variable to notify the CAN thread to spin a new cycle.
  std::condition_variable can_wait_condition_;

  //! True if and only if a CAN communication cycle is under way.
  bool ongoing_can_cycle_ = false;

  //! CAN thread exits when it is notified and this boolean is true.
  bool done_ = false;

  //! Callback function called upon completion of a CAN cycle
  std::function<void(const moteus::Output&)> callback_;

  //! Buffer to read commands from and write replies to.
  moteus::Data data_;

  //! Thread for CAN communication cycles
  std::thread can_thread_;

  //! Internal Pi3Hat interface. Only use from the CAN thread.
  std::unique_ptr<Pi3Hat> pi3hat_;

  // These are kept persistently so that no memory allocation is
  // required in steady state.
  std::vector<::mjbots::pi3hat::CanFrame> tx_can_;
  std::vector<::mjbots::pi3hat::CanFrame> rx_can_;

  //! Latest attitude read from the pi3hat
  ::mjbots::pi3hat::Attitude attitude_;
};

}  // namespace vulp::actuation
