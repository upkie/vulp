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
inline void observe_time(Dictionary& observation) {
  const auto now = std::chrono::system_clock::now();
  const auto time_since_epoch = now.time_since_epoch();
  const auto nb_us = duration_cast<microseconds>(time_since_epoch).count();
  observation("time") = static_cast<double>(nb_us) / 1e6;
}

}  // namespace vulp::observation
