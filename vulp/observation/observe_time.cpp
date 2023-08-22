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
