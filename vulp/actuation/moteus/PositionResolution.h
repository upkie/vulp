// Copyright 2022 Stéphane Caron
// SPDX-License-Identifier: Apache-2.0
/*
 * This file incorporates work covered by the following copyright and
 * permission notice:
 *
 *     Copyright 2020 Josh Pieper, jjp@pobox.com.
 *     SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "vulp/actuation/moteus/Resolution.h"

namespace vulp::actuation::moteus {

struct PositionResolution {
  Resolution position = Resolution::kFloat;
  Resolution velocity = Resolution::kFloat;
  Resolution feedforward_torque = Resolution::kFloat;
  Resolution kp_scale = Resolution::kFloat;
  Resolution kd_scale = Resolution::kFloat;
  Resolution maximum_torque = Resolution::kIgnore;
  Resolution stop_position = Resolution::kFloat;
  Resolution watchdog_timeout = Resolution::kFloat;
};

}  // namespace vulp::actuation::moteus
