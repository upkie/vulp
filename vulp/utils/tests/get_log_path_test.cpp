// SPDX-License-Identifier: Apache-2.0
// Copyright 2022 Stéphane Caron

#include "vulp/utils/get_log_path.h"

#include <string>

#include "gtest/gtest.h"

namespace vulp::utils {

TEST(GetLogPath, StartsWithlogDir) {
  auto log_path = get_log_path("foo_spine", "bar_dir");
  ASSERT_EQ(now.rfind("bar", 0), 0);
}

}  // namespace vulp::utils
