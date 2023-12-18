// Copyright 2022 Stéphane Caron
// SPDX-License-Identifier: Apache-2.0

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
