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

#include "gtest/gtest.h"
#include "vulp/utils/LowPassFilter.h"

namespace vulp::utils {

TEST(LowPassFilter, Reset) {
  constexpr double alpha = 0.42;
  LowPassFilter<double> filter(/* alpha = */ alpha);
  ASSERT_DOUBLE_EQ(filter.alpha(), alpha);
  ASSERT_DOUBLE_EQ(filter.get(), 0.);
  filter.reset(2.);
  ASSERT_DOUBLE_EQ(filter.get(), 2.);
}

TEST(LowPassFilter, AlphaIsOne) {
  LowPassFilter<double> filter(/* alpha = */ 1.0);
  filter.step(5.);
  ASSERT_DOUBLE_EQ(filter.get(), 5.);
}

TEST(LowPassFilter, Converge) {
  constexpr double target = 1.;
  LowPassFilter<double> filter(/* alpha = */ 0.1);
  for (unsigned i = 0; i < 20; i++) {
    filter.step(target);
  }
  ASSERT_GT(filter.get(), 0.8);
  ASSERT_LT(filter.get(), target);
}

}  // namespace vulp::utils
