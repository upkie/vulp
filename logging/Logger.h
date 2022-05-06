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

#pragma once

#include <palimpsest/Dictionary.h>

#include <fstream>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "vulp/utils/CircularBuffer.h"

namespace vulp::logging {

using palimpsest::Dictionary;
using vulp::utils::CircularBuffer;

/*! Number of slots in the internal circular buffer.
 *
 * The buffer copes with variability in the rate of writing to the output file.
 * The circular buffer stores values in RAM when the writing rate drops and
 * empties when the writing rate again exceeds the input rate. Yes, the input
 * rate has to be lower than the writing rate. There's no way around that ;)
 */
constexpr size_t kBufferSize = 1024;

/*! Multi-threaded logger.
 *
 * This logger stores input data to an internal circular memory buffer and
 * flushes it to an output file in a separate thread.
 *
 * \note The input rate of a logger, i.e. the rate in MB/s at which its write
 * function is called (data size / duration between calls), should always be
 * lower than the writing rate (a.k.a. speed) of the filesystem. Otherwise
 * circular buffer will likely fill to capacity and start dropping data.
 *
 * Quick performance check for the writing rate:
 *
 * ```
 * dd if=/dev/random of=somefile bs=10M count=100
 * ```
 */
class Logger {
 public:
  /*! Open a new log file and start the logging thread.
   *
   * \param[in] path Path to the output log file.
   * \param[in] append If true, append to the log file if it exists.
   */
  explicit Logger(const std::string &path, bool append = false);

  //! Stop the logging thread.
  ~Logger();

  /*! Write data to the log.
   *
   * This function only copies the data to the circular buffer. It is then
   * written later to file by the logging thread.
   *
   * \param data Raw data to write.
   * \param size Size of data array.
   *
   * \return Whether the item was successfully stored in the circular buffer
   *     (false means the buffer is full).
   */
  bool write(char *data, size_t size);

  /*! Write a dictionary to the log.
   *
   * \param dict Dictionary to write.
   *
   * \return True if the dictionary was successfully stored for logging.
   *
   * \sa write(char*, size_t)
   */
  inline bool write(const Dictionary &dict) {
    size_t size = dict.serialize(serialization_buffer_);
    return Logger::write(serialization_buffer_.data(), size);
  }

  //! Size of last written message, in number of bytes.
  size_t last_write_size() const { return last_write_size_; }

 private:
  //! Flush internal buffer to output file.
  void flush_buffer();

 private:
  //! Output file stream.
  std::ofstream file_;

  //! Logging thread.
  std::thread thread_;

  //! Run flag for the thread loop.
  bool keep_going_ = true;

  //! Circular buffer holding serialized data to write to the log.
  CircularBuffer<std::pair<char *, size_t>, kBufferSize> circular_buffer_;

  //! Last item popped from the circular buffer.
  std::pair<char *, size_t> pop_;

  /*! Buffer used by \ref write(const Dictionary&).
   *
   * This buffer is here for convenience. A consumer managing its own buffer
   * can call \ref write(char*, size_t) directly, then this buffer won't be
   * used.
   */
  std::vector<char> serialization_buffer_;

  //! Size of last written message, in number of bytes.
  size_t last_write_size_ = 0;
};

}  // namespace vulp::logging
