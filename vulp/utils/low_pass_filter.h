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

#include <stdexcept>
#include <string>

namespace vulp::utils {

//! Exception for filter errors
class FilterError : public std::runtime_error {
 public:
  /*! Constructor.
   *
   * \param[in] message Error message.
   */
  explicit FilterError(const std::string& message) throw()
      : std::runtime_error(message.c_str()) {}

  /*! Get error message.
   *
   * \return Error message.
   */
  const char* what() const throw() { return std::runtime_error::what(); }
};

/*! Low-pass filter as an inline function.
 *
 * \param prev_output Previous filter output, or initial value.
 * \param cutoff_period Cutoff period in [s].
 * \param new_input New filter input.
 * \param dt Sampling period in [s].
 *
 * \return New filter output.
 */
inline double low_pass_filter(double prev_output, double cutoff_period,
                              double new_input, double dt) {
  // Make sure the cutoff period is not too small
  if (cutoff_period <= 2.0 * dt) {
    auto message =
        std::string("[low_pass_filter] Cutoff period ") +
        std::to_string(cutoff_period) +
        " s is less than 2 * dt = " + std::to_string(2.0 * dt) +
        " s, causing information loss (Nyquist–Shannon sampling theorem)";
    throw FilterError(message);
  }

  // Actual filtering ;)
  const double alpha = dt / cutoff_period;
  return prev_output + alpha * (new_input - prev_output);
}

}  // namespace vulp::utils
