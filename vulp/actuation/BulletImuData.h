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

#include <string>

#include "RobotSimulator/b3RobotSimulatorClientAPI.h"
#include "vulp/actuation/ImuData.h"

namespace vulp::actuation {

struct BulletImuData : public ImuData {
  //! Spatial linear velocity in [m] / [s]², used to compute the acceleration
  Eigen::Vector3d linear_velocity_imu_in_world = Eigen::Vector3d::Zero();
};

}  // namespace vulp::actuation
