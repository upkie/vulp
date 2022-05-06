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
 *     Logger class of mc_rtc
 *     Copyright 2015-2019 CNRS-UM LIRMM, CNRS-AIST JRL
 *     License: BSD-2-Clause (see licenses/LICENSE-mc_rtc)
 */

#include "vulp/logging/Logger.h"

#include <string>

namespace vulp::logging {

Logger::Logger(const std::string &path, bool append) {
  auto flags = std::ofstream::binary;
  if (append) {
    flags |= std::ofstream::app;
  }
  file_.open(path, flags);
  if (!file_) {
    throw std::runtime_error("Cannot open " + path + " for writing");
  }
  thread_ = std::thread([this]() {
    pthread_setname_np(pthread_self(), "logger_thread");
    while (keep_going_) {
      flush_buffer();

      // The sleep duration doesn't have a big influence, no need to wake up
      // too often. This value assumes a > 1 ms long control period.
      std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
    flush_buffer();
  });
}

Logger::~Logger() {
  keep_going_ = false;
  if (thread_.joinable()) {
    thread_.join();
  }
  file_.close();
}

bool Logger::write(char *data, size_t size) {
  last_write_size_ = size;
  char *ndata = new char[size];
  std::memcpy(ndata, data, size);
  return circular_buffer_.push({ndata, size});
}

void Logger::flush_buffer() {
  while (circular_buffer_.pop(pop_)) {
    char *data = pop_.first;
    size_t size = pop_.second;
    file_.write(data, static_cast<int>(size));
    delete[] data;

    // Also flush the output stream, otherwise buffering tends to increase
    // variance / cause spikes in consumer loop rates.
    file_.flush();
  }
}

}  // namespace vulp::logging
