// Copyright 2022 Stéphane Caron
// SPDX-License-Identifier: Apache-2.0

#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "vulp/observation/Observer.h"

namespace vulp::observation::tests {

TEST(Observer, BaseClassObservesNothing) {
  Observer observer;
  Dictionary observation;
  ASSERT_TRUE(observation.is_empty());
  observer.read(observation);
  observer.write(observation);
  ASSERT_TRUE(observation.is_empty());
}

}  // namespace vulp::observation::tests
