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

#include <palimpsest/Dictionary.h>

#include <map>
#include <string>
#include <vector>

#include "vulp/actuation/moteus/protocol.h"

namespace vulp::observation {

/*! Observe servo measurements.
 *
 * \param[out] observation Dictionary to write observations to.
 * \param[in] servo_joint_map Map from servo ID to joint name.
 * \param[in] servo_replies List of servo replies from the CAN bus.
 */
void observe_servos(
    palimpsest::Dictionary& observation,
    const std::map<int, std::string>& servo_joint_map,
    const std::vector<actuation::moteus::ServoReply>& servo_replies);

}  // namespace vulp::observation
