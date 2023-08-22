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

#include <cstdint>

//! Utility functions.
namespace vulp::utils {

//! Mathematical functions.
namespace math {

//! True if and only if \p divisor divides \p number.
inline bool divides(uint32_t number, uint32_t divisor) {
  if (divisor == 0) {
    return false;
  }
  uint32_t k = number / divisor;
  return (number == k * divisor);
}

}  // namespace math

}  // namespace vulp::utils
