// SPDX-License-Identifier: Apache-2.0
// Copyright 2022 Stéphane Caron

#include "vulp/utils/get_log_path.h"

#include <string>

#include "gtest/gtest.h"

namespace vulp::utils {

TEST(GetLogPath, StartsWithLogdir) {
  const std::string path = get_log_path("foo_spine", "bar_dir");
  ASSERT_EQ(path.rfind("bar_dir", 0), 0);
}

TEST(GetLogPath, EndsWithMpack) {
  const std::string path = get_log_path("foo_spine", "bar_dir");
  const std::string suffix = ".mpack";
  ASSERT_EQ(path.rfind(suffix), path.size() - suffix.size());
}

}  // namespace vulp::utils
