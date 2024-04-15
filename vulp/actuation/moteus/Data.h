// SPDX-License-Identifier: Apache-2.0
// Copyright 2022 Stéphane Caron
/*
 * This file incorporates work covered by the following copyright and
 * permission notice:
 *
 *     Copyright 2020 Josh Pieper, jjp@pobox.com.
 *     SPDX-License-Identifier: Apache-2.0
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
