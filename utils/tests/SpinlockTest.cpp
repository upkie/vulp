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

#include <thread>

#include "gtest/gtest.h"
#include "vulp/utils/Spinlock.h"

namespace vulp::utils {

TEST(Spinlock, AcquireAndRelease) {
  Spinlock lock;
  ASSERT_NO_THROW(lock.acquire());
  ASSERT_NO_THROW(lock.release());
}

TEST(Spinlock, TwoThreads) {
  Spinlock lock;
  lock.acquire();
  unsigned step = 0;

  auto thread = std::thread([&lock, &step]() {
    lock.acquire();
    step = 42;
    lock.release();
  });

  ASSERT_EQ(step, 0);
  ASSERT_NO_THROW(lock.release());
  ASSERT_NO_THROW(thread.join());
  ASSERT_NO_THROW(lock.acquire());
  ASSERT_EQ(step, 42);
}

}  // namespace vulp::utils
