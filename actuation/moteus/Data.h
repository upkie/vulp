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
 *     License: Apache-2.0
 */

#pragma once

#include "vulp/actuation/moteus/ServoCommand.h"
#include "vulp/actuation/moteus/ServoReply.h"
#include "vulp/actuation/moteus/Span.h"

namespace vulp::actuation::moteus {

/*! Data for one control cycle.
 *
 * This describes what you would like to do in a given control cycle in terms
 * of sending commands or querying data.
 */
struct Data {
  //! Pointers to servo command buffers.
  Span<ServoCommand> commands;

  //! Pointers to servo reply buffers.
  Span<ServoReply> replies;
};

}  // namespace vulp::actuation::moteus
