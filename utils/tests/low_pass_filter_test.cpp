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

#include "vulp/utils/low_pass_filter.h"

#include "gtest/gtest.h"

namespace vulp::utils {

TEST(InlineLowPassFilter, InformationLoss) {
  const double prev_output = 0.0;
  const double dt = 1.23456789;
  const double input = 5.0;
  ASSERT_THROW(low_pass_filter(prev_output, dt, input, dt), FilterError);
}

TEST(InlineLowPassFilter, Converges) {
  const double target = 1.0;
  const double dt = 1e-3;
  const double T = 10 * dt;
  double output = 0.0;
  for (unsigned i = 0; i < 20; i++) {
    output = low_pass_filter(output, T, target, dt);
  }
  ASSERT_GT(output, 0.8);
  ASSERT_LT(output, target);
}

}  // namespace vulp::utils
