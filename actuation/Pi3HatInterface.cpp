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
 *
 * This file incorporates work covered by the following copyright and
 * permission notice:
 *
 *     Copyright 2020 Josh Pieper, jjp@pobox.com.
 *     License: Apache-2.0
 */

#include "vulp/actuation/Pi3HatInterface.h"

namespace vulp::actuation {

Pi3HatInterface::Pi3HatInterface(const ServoLayout& layout, const int can_cpu,
                                 const Pi3Hat::Configuration& pi3hat_config)
    : Interface(layout),
      can_cpu_(can_cpu),
      pi3hat_config_(pi3hat_config),
      can_thread_(std::bind(&Pi3HatInterface::run_can_thread, this)) {}

Pi3HatInterface::~Pi3HatInterface() {
  done_ = true;  // comes first
  if (ongoing_can_cycle_) {
    // thread will exit at the end of its loop because done_ is set
    spdlog::info("Waiting for CAN thread to finish active cycle...");
    can_thread_.join();
    spdlog::info("CAN thread finished last cycle cleanly");
  } else /* thread is waiting for notification */ {
    {
      std::lock_guard<std::mutex> lock(mutex_);
      can_wait_condition_.notify_one();
    }
    can_thread_.join();
  }
}

void Pi3HatInterface::reset(const palimpsest::Dictionary& config) {}

void Pi3HatInterface::cycle(
    const moteus::Data& data,
    std::function<void(const moteus::Output&)> callback) {
  std::lock_guard<std::mutex> lock(mutex_);
  if (ongoing_can_cycle_) {
    throw std::logic_error(
        "Cycle cannot be called before the previous one has completed.");
  }

  callback_ = std::move(callback);
  ongoing_can_cycle_ = true;
  data_ = data;

  can_wait_condition_.notify_all();
}

void Pi3HatInterface::run_can_thread() {
  vulp::utils::configure_cpu(can_cpu_);
  vulp::utils::configure_scheduler(10);
  pi3hat_.reset(new Pi3Hat({pi3hat_config_}));
  pthread_setname_np(pthread_self(), "can_thread");
  while (!done_) {
    {
      std::unique_lock<std::mutex> lock(mutex_);
      if (!ongoing_can_cycle_) {
        can_wait_condition_.wait(lock);
        if (done_) {
          return;
        }
        if (!ongoing_can_cycle_) {
          continue;
        }
      }
    }
    auto output = cycle_can_thread();
    std::function<void(const moteus::Output&)> callback_copy;
    {
      std::unique_lock<std::mutex> lock(mutex_);
      ongoing_can_cycle_ = false;
      std::swap(callback_copy, callback_);
    }
    callback_copy(output);
  }
}

moteus::Output Pi3HatInterface::cycle_can_thread() {
  tx_can_.resize(data_.commands.size());
  int out_idx = 0;
  for (const auto& cmd : data_.commands) {
    const auto& query = cmd.query;

    auto& can = tx_can_[out_idx++];

    can.expect_reply = query.any_set();
    can.id = cmd.id | (can.expect_reply ? 0x8000 : 0x0000);
    can.size = 0;

    can.bus = [&]() {
      const auto it = servo_bus_map().find(cmd.id);
      if (it == servo_bus_map().end()) {
        return 1;
      }
      return it->second;
    }();

    moteus::WriteCanFrame write_frame(can.data, &can.size);
    switch (cmd.mode) {
      case moteus::Mode::kStopped: {
        moteus::EmitStopCommand(&write_frame);
        break;
      }
      case moteus::Mode::kPosition:
      case moteus::Mode::kZeroVelocity: {
        moteus::EmitPositionCommand(&write_frame, cmd.position, cmd.resolution);
        break;
      }
      default: {
        throw std::logic_error("unsupported mode");
      }
    }
    moteus::EmitQueryCommand(&write_frame, cmd.query);
  }

  rx_can_.resize(data_.commands.size() * 2);

  Pi3Hat::Input input;
  input.tx_can = {tx_can_.data(), tx_can_.size()};
  input.rx_can = {rx_can_.data(), rx_can_.size()};
  input.attitude = &attitude_;
  input.request_attitude = true;
  input.wait_for_attitude = true;  // we may revisit this later

  moteus::Output result;
  const auto pi3hat_output = pi3hat_->Cycle(input);
  if (pi3hat_output.error) {
    spdlog::error("pi3hat: {}", pi3hat_output.error);
  }
  for (size_t i = 0; i < pi3hat_output.rx_can_size && i < data_.replies.size();
       ++i) {
    const auto& can = rx_can_[i];
    data_.replies[i].id = (can.id & 0x7f00) >> 8;
    data_.replies[i].result = moteus::ParseQueryResult(can.data, can.size);
    result.query_result_size = i + 1;
  }
  if (!pi3hat_output.attitude_present) {  // because we wait for attitude
    spdlog::warn("Missing attitude data!");
  }
  return result;
}

}  // namespace vulp::actuation
