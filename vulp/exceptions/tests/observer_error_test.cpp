// SPDX-License-Identifier: Apache-2.0
// Copyright 2022 Stéphane Caron

#include <Eigen/Geometry>
#include <map>
#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "vulp/exceptions/ObserverError.h"

namespace vulp::exceptions::tests {

TEST(ObserverError, NineteenSixtyWho) {
  const std::string prefix = "nineteen_sixty";
  const std::string key = "what";
  ObserverError error(prefix, key);
  const std::string msg = error.what();
  ASSERT_NE(msg.find(prefix), std::string::npos);
  ASSERT_NE(msg.find(key), std::string::npos);
}

}  // namespace vulp::exceptions::tests
