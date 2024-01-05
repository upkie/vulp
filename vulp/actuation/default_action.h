// SPDX-License-Identifier: Apache-2.0
// Copyright 2022 Stéphane Caron

#pragma once

namespace vulp::actuation {

namespace default_action {

constexpr double kFeedforwardTorque = 0.0;
constexpr double kPosition = std::numeric_limits<double>::quiet_NaN();
constexpr double kVelocity = 0.0;
constexpr double kKpScale = 1.0;
constexpr double kKdScale = 1.0;
constexpr double kMaximumTorque = 1.0;  // N.m

}  // namespace default_action

}  // namespace vulp::actuation
