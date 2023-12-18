// Copyright 2022 Stéphane Caron
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <palimpsest/Dictionary.h>

namespace vulp::observation {

using palimpsest::Dictionary;

/*! Observe time since the epoch.
 *
 * \param[out] observation Dictionary to write observations to.
 */
void observe_time(Dictionary& observation);

}  // namespace vulp::observation
