// SPDX-License-Identifier: Apache-2.0
/*
 * Copyright 2022 Stéphane Caron
 */

#include "vulp/utils/realtime.h"

#include "gtest/gtest.h"

namespace vulp::utils {

TEST(Realtime, ConfigureCPU) { ASSERT_NO_THROW(configure_cpu(0)); }

TEST(Realtime, ConfigureScheduler) {
  constexpr int priority = 10;
  ASSERT_THROW(configure_scheduler(priority), std::runtime_error);
}

}  // namespace vulp::utils
