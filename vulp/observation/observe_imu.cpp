// SPDX-License-Identifier: Apache-2.0
// Copyright 2022 Stéphane Caron

#include "vulp/observation/observe_imu.h"

using palimpsest::Dictionary;

namespace vulp::observation {

void observe_imu(Dictionary& observation, const actuation::ImuData& imu_data) {
  // Eigen quaternions are serialized as [w, x, y, z]
  // See include/palimpsest/mpack/eigen.h in @palimpsest
  observation("imu")("orientation") = imu_data.orientation_imu_in_ars;
  observation("imu")("angular_velocity") = imu_data.angular_velocity_imu_in_imu;
  observation("imu")("linear_acceleration") =
      imu_data.linear_acceleration_imu_in_imu;
}

}  // namespace vulp::observation
