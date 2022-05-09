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

#include <atomic>

namespace vulp::utils {

//! Simple spinlock with PAUSE hint.
class Spinlock {
 public:
  //! Lock is initially free.
  Spinlock() : flag_(0) {}

  //! Check if the lock is free, and if not wait for it.
  void acquire() noexcept {
    while (true) {
      if (!flag_.exchange(true, std::memory_order_acquire)) {
        return;
      }
      while (flag_.load(std::memory_order_relaxed)) {
#if defined(__x86_64__) || defined(__i386__)
        // Reduce contention between hardware threads (SMT)
        // See https://www.felixcloutier.com/x86/pause
        // This is a pause instruction + memory fence
        __builtin_ia32_pause();  // LCOV_EXCL_LINE
#elif defined(__arm__) || defined(__aarch64__)
        __asm__ __volatile__("yield");
#else
#error "What is the equivalent of an x86 PAUSE / ARM YIELD for this CPU?"
#endif
      }
    }
  }

  //! Release lock.
  void release() noexcept { flag_.store(false, std::memory_order_release); }

 private:
  //! Atomic boolean flag, true when locked and false when released.
  std::atomic<bool> flag_;
};

}  // namespace vulp::utils
