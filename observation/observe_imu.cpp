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

#include "vulp/observation/observe_imu.h"

using palimpsest::Dictionary;

namespace vulp::observation {

void observe_imu(Dictionary& observation, const actuation::ImuData& imu_data) {
  // Eigen quaternions are serialized as [w, x, y, z]
  // See include/palimpsest/mpack/eigen.h in @palimpsest
  observation("imu")("orientation") = imu_data.orientation_imu_in_world;
  observation("imu")("angular_velocity") = imu_data.angular_velocity_imu_in_imu;
  observation("imu")("linear_acceleration") =
      imu_data.linear_acceleration_imu_in_imu;
}

}  // namespace vulp::observation
