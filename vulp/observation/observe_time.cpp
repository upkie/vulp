// SPDX-License-Identifier: Apache-2.0
/*
 * Copyright 2022 Stéphane Caron
 */

#include "vulp/observation/observe_time.h"

namespace vulp::observation {

using std::chrono::duration_cast;
using std::chrono::microseconds;

/*! Observe time since the epoch.
 *
 * \param[out] observation Dictionary to write observations to.
 */
void observe_time(Dictionary& observation) {
  const auto now = std::chrono::system_clock::now();
  const auto time_since_epoch = now.time_since_epoch();
  const auto nb_us = duration_cast<microseconds>(time_since_epoch).count();
  observation("time") = static_cast<double>(nb_us) / 1e6;
}

}  // namespace vulp::observation
