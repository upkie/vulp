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

#include "vulp/spine/AgentInterface.h"

#include <spdlog/spdlog.h>
#include <unistd.h>

namespace vulp::spine {

/*! Allocate file with some error handling.
 *
 * \param[in] file_descriptor File descriptor.
 * \param[in] bytes Number of bytes to allocate.
 */
void allocate_file(int file_descriptor, int bytes) {
  struct ::stat file_stats;

  if (::ftruncate(file_descriptor, bytes) < 0) {
    throw std::runtime_error("Error truncating file, errno is " +
                             std::to_string(errno));
  }

  ::fstat(file_descriptor, &file_stats);
  if (file_stats.st_size < bytes) {
    throw std::runtime_error(
        "Error allocating " + std::to_string(bytes) +
        " bytes in shared memory. Errno is : " + std::to_string(errno));
  }
}

AgentInterface::AgentInterface(const std::string& name, size_t size)
    : name_(name), size_(size) {
  // Allocate shared memory
  // About umask: see https://stackoverflow.com/a/11909753
  mode_t existing_umask = ::umask(0);
  int file_descriptor =
      ::shm_open(name.c_str(), O_RDWR | O_CREAT | O_EXCL, 0666);
  ::umask(existing_umask);
  if (file_descriptor < 0) {
    if (errno == EINVAL) {
      spdlog::error("Cannot open shared memory \"{}\": file name is invalid.",
                    name);
    } else if (errno == EEXIST) {
      spdlog::error(
          "Cannot open shared memory \"{}\": file already exists. Is the spine "
          "already running? Did it not exit properly? If not, run "
          "``./tools/bazelisk run "
          "@vulp//tools/shm:clean -- {}``",
          name, name);
    } else {
      spdlog::error("Cannot open shared memory file: errno is {}", errno);
    }
    throw std::runtime_error("Error opening file");
  }
  allocate_file(file_descriptor, size);

  // Map shared memory
  mmap_ = ::mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED,
                 file_descriptor, 0);
  if (mmap_ == MAP_FAILED) {
    throw std::runtime_error("Error mapping file");
  }
  if (::close(file_descriptor) < 0) {
    throw std::runtime_error("Error closing file descriptor");
  }

  // Initialize internal pointers
  mmap_request_ = static_cast<uint32_t*>(mmap_);
  mmap_size_ = static_cast<uint32_t*>(mmap_request_ + 1);
  mmap_data_ = reinterpret_cast<char*>(mmap_size_ + 1);
}

AgentInterface::~AgentInterface() {
  if (::munmap(mmap_, size_) < 0) {
    spdlog::error("Failed to unmap memory; errno is {}", errno);
  }
  if (::shm_unlink(name_.c_str()) < 0) {
    spdlog::error("Failed to unlink shared memory; errno is {}", errno);
  }
}

void AgentInterface::set_request(Request request) {
  *mmap_request_ = static_cast<uint32_t>(request);
}

void AgentInterface::write(char* data, size_t size) {
  if (size_ <= size + 2 * sizeof(uint32_t)) {
    spdlog::error(
        "Trying to write {} bytes to agent interface buffer of size {} bytes",
        size, size_);
    throw std::runtime_error("Agent interface buffer overflow");
  }
  *mmap_size_ = size;
  std::memcpy(mmap_data_, data, size);
}

}  // namespace vulp::spine
