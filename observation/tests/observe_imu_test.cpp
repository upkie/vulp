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

#include <palimpsest/Dictionary.h>

#include <Eigen/Geometry>
#include <map>
#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "vulp/actuation/ImuData.h"

using palimpsest::Dictionary;

namespace vulp::observation::tests {

TEST(IMU, ReadOrientation) {
  actuation::ImuData imu_data;
  imu_data.orientation_imu_in_ars = Eigen::Quaterniond(0., 1., 0., 0.);

  Dictionary observation;
  observe_imu(observation, imu_data);
  ASSERT_TRUE(observation.has("imu"));
  ASSERT_TRUE(observation("imu").has("orientation"));
  ASSERT_TRUE(observation("imu")("orientation")
                  .as<Eigen::Quaterniond>()
                  .isApprox(imu_data.orientation_imu_in_ars));
}

}  // namespace vulp::observation::tests
