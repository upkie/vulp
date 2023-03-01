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

#include <string>

#include "vulp/observation/Source.h"

namespace vulp::observation::sources {

//! Characters required to read the temperature in [mC] from the kernel.
constexpr unsigned kCpuTemperatureBufferSize = 12;

//! Source for CPU temperature readings.
class CpuTemperature : public Source {
 public:
  /*! Open file to query temperature from the kernel.
   *
   * \param[in] temp_path Path to thermal-zone special file from the Linux
   * kernel.
   */
  CpuTemperature(
      const char* temp_path = "/sys/class/thermal/thermal_zone0/temp");

  //! Close file.
  ~CpuTemperature() override;

  //! Prefix of output in the observation dictionary.
  inline std::string prefix() const noexcept final { return "cpu_temperature"; }

  /*! Write output to a dictionary.
   *
   * \param[out] observation Dictionary to write observations to.
   */
  void write(Dictionary& observation) final;

  //! Check if temperature observations are disabled.
  bool is_disabled() const { return is_disabled_; }

 private:
  /*! Issue a warning if the temperature is too high.
   *
   * \param[in] temperature Current temperature in °C.
   *
   * This warning is rather specific to the Raspberry Pi.
   */
  void check_temperature_warning(const double temperature) noexcept;

 private:
  //! Write function does nothing if this flag is set.
  bool is_disabled_ = false;

  //! File descriptor to the kernel virtual file.
  int fd_;

  //! Buffer to hold the temperature string read from file.
  char buffer_[kCpuTemperatureBufferSize];

  //! Flag set to true when issuing a temperature warning.
  bool has_warned_;
};

}  // namespace vulp::observation::sources
