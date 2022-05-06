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

#include "vulp/observation/CpuTemperature.h"

namespace vulp::observation {

CpuTemperature::CpuTemperature() : has_warned_(false) {
  fd_ = ::open("/sys/class/thermal/thermal_zone0/temp", O_RDONLY | O_NONBLOCK);
  ::memset(buffer_, 0, sizeof(buffer_));
}

CpuTemperature::~CpuTemperature() { ::close(fd_); }

void CpuTemperature::write(Dictionary& observation) {
  ssize_t size = ::pread(fd_, buffer_, kCpuTemperatureBufferSize, 0);
  if (size <= 0) {
    spdlog::warn("Read {} bytes from temperature file", size);
    return;
  }
  const double temperature = std::stol(buffer_) / 1000.;
  check_temperature_warning(temperature);
  auto& output = observation(prefix());
  output = temperature;
}

void CpuTemperature::check_temperature_warning(
    const double temperature) noexcept {
  constexpr double kConcerningTemperature = 75.0;
  if (temperature > kConcerningTemperature) {
    if (!has_warned_) {
      spdlog::warn("CPU temperature > {} °C, thermal throttling may occur",
                   kConcerningTemperature);
      has_warned_ = true;
    }
  }

  constexpr double kHysteresisFactor = 0.95;
  if (has_warned_ && temperature < kHysteresisFactor * kConcerningTemperature) {
    has_warned_ = false;
  }
}

}  // namespace vulp::observation
