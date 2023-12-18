// Copyright 2022 Stéphane Caron
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <string>

#include "RobotSimulator/b3RobotSimulatorClientAPI.h"
#include "vulp/actuation/ImuData.h"

namespace vulp::actuation {

struct BulletImuData : public ImuData {
  //! Spatial linear velocity in [m] / [s]², used to compute the acceleration
  Eigen::Vector3d linear_velocity_imu_in_world = Eigen::Vector3d::Zero();
};

}  // namespace vulp::actuation
