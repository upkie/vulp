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
