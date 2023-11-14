// Copyright 2023 Inria
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <string>

#include "RobotSimulator/b3RobotSimulatorClientAPI.h"
#include "vulp/actuation/ImuData.h"

namespace vulp::actuation {

struct BulletJointProperties {
  //! Kinetic friction, in N.m
  double friction = 0.0;

  //! Maximum torque, in N.m
  double maximum_torque = 0.0;
};

}  // namespace vulp::actuation

