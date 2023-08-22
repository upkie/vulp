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

#include <limits>

namespace vulp::actuation::moteus {

/*! Position command.
 *
 * \note Default values here are also the initial joint values for the mock
 * interface. See the documentation at \ref
 * vulp::actuation::MockInterface::ServoState::step.
 */
struct PositionCommand {
  double position = 0.0;
  double velocity = 0.0;
  double feedforward_torque = 0.0;
  double kp_scale = 1.0;
  double kd_scale = 1.0;
  double maximum_torque = 0.0;
  double stop_position = std::numeric_limits<double>::quiet_NaN();
  double watchdog_timeout = 0.0;
};

}  // namespace vulp::actuation::moteus
