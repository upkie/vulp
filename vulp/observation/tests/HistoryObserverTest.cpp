// SPDX-License-Identifier: Apache-2.0
// Copyright 2024 Inria

#include <palimpsest/Dictionary.h>
#include <palimpsest/exceptions/KeyError.h>

#include <memory>

#include "gtest/gtest.h"
#include "vulp/observation/HistoryObserver.h"

namespace vulp::observation::tests {

using palimpsest::Dictionary;

TEST(HistoryObserver, DoubleInitialization) {
  HistoryObserver<double> history_observer(
      /* keys = */ std::vector<std::string>{"servo", "left_knee", "torque"},
      /* size = */ 3,
      /* default_value = */ -1.0);

  Dictionary observation;

  history_observer.write(observation);
  const auto& values = observation("history")("servo")("left_knee")("torque")
                           .as<std::vector<double>>();
  ASSERT_EQ(values.size(), 3);
  ASSERT_DOUBLE_EQ(values[0], -1.0);
  ASSERT_DOUBLE_EQ(values[1], -1.0);
  ASSERT_DOUBLE_EQ(values[2], -1.0);
}

TEST(HistoryObserver, DoubleReadThenWrite) {
  HistoryObserver<double> history_observer(
      /* keys = */ std::vector<std::string>{"servo", "left_knee", "torque"},
      /* size = */ 3,
      /* default_value = */ std::numeric_limits<double>::quiet_NaN());

  Dictionary observation;
  observation("servo")("left_knee")("torque") = 3.0;
  history_observer.read(observation);
  observation("servo")("left_knee")("torque") = 2.0;
  history_observer.read(observation);
  observation("servo")("left_knee")("torque") = 1.0;
  history_observer.read(observation);

  history_observer.write(observation);
  const auto& values = observation("history")("servo")("left_knee")("torque")
                           .as<std::vector<double>>();
  ASSERT_EQ(values.size(), 3);
  ASSERT_DOUBLE_EQ(values[0], 1.0);  // most recent comes first
  ASSERT_DOUBLE_EQ(values[1], 2.0);
  ASSERT_DOUBLE_EQ(values[2], 3.0);
}

}  // namespace vulp::observation::tests
