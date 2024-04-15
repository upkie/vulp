// SPDX-License-Identifier: Apache-2.0
// Copyright 2022 Stéphane Caron

#include <map>

#include "gtest/gtest.h"
#include "vulp/actuation/MockInterface.h"
#include "vulp/actuation/moteus/protocol.h"
#include "vulp/actuation/tests/coffee_machine_layout.h"

namespace vulp::actuation {

TEST(MockInterfaceTest, CycleCallsCallback) {
  const auto layout = get_coffee_machine_layout();
  const double dt = 1e-3;
  MockInterface interface(layout, dt);

  moteus::Data data;
  bool callback_called = false;
  interface.cycle(data, [&callback_called](const moteus::Output& output) {
    callback_called = true;
  });
  ASSERT_TRUE(callback_called);
}

}  // namespace vulp::actuation
