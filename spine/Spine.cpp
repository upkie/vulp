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
 *     License: Apache-2.0 (see licenses/LICENSE-pi3hat)
 */

#include "vulp/spine/Spine.h"

#include <mpacklog/Logger.h>

#include <limits>

#include "vulp/observation/ObserverError.h"

namespace vulp::spine {

using observation::ObserverError;
using palimpsest::Dictionary;

/*! Expect a timestamp in the action dictionary.
 *
 * \note We expect action/brain_time to be present so that we can run combine
 * brain and spine logs. NB: this operation has been deprecated. However the
 * mapping could still be useful for database purposes. Keeping it for now.
 */
inline void expect_timestamp(Dictionary& action) {
  action("brain_time") = std::numeric_limits<double>::quiet_NaN();
}

Spine::Spine(const Parameters& params, actuation::Interface& actuation,
             observation::ObserverPipeline& observers)
    : frequency_(params.frequency),
      actuation_(actuation),
      agent_interface_(params.shm_name, params.shm_size),
      observer_pipeline_(observers),
      logger_(params.log_path),
      caught_interrupt_(vulp::utils::handle_interrupts()),
      state_machine_(agent_interface_),
      state_cycle_beginning_(State::kOver),
      state_cycle_end_(State::kOver) {
// Thread name as it appears in the `cmd` column of `ps`
#ifdef __APPLE__
  pthread_setname_np("spine_thread");
#else
  pthread_setname_np(pthread_self(), "spine_thread");
#endif

  // Real-time configuration
  // NB: it is too late to lock memory here, this should be done by the caller
  if (params.cpu >= 0) {
    utils::configure_cpu(params.cpu);
    utils::configure_scheduler(10);
  }

  // Inter-process communication
  agent_interface_.set_request(Request::kNone);

  // Initialize internal dictionary
  Dictionary& observation = dict_("observation");
  observation::observe_time(observation);
  dict_.insert<double>("time", observation.get<double>("time"));
}

void Spine::reset(const Dictionary& config) {
  actuation_.reset(config);
  dict_("action").clear();
  expect_timestamp(dict_("action"));
  actuation_.initialize_action(dict_("action"));
  observer_pipeline_.reset(config);
}

void Spine::log_dict() {
  Dictionary& spine = dict_("spine");
  spine("logger")("last_size") = logger_.last_size();
  spine("state")("cycle_beginning") =
      static_cast<uint32_t>(state_cycle_beginning_);
  spine("state")("cycle_end") = static_cast<uint32_t>(state_cycle_end_);
  logger_.put(dict_);
}

void Spine::run() {
  Dictionary& spine = dict_("spine");
  utils::SynchronousClock clock(frequency_);
  while (state_machine_.state() != State::kOver) {
    cycle();
    if (state_machine_.state() != State::kSendStops) {
      spine("clock")("measured_period") = clock.measured_period();
      spine("clock")("skip_count") = clock.skip_count();
      spine("clock")("slack") = clock.slack();
      log_dict();
    }
    clock.wait_for_next_tick();
  }
  spdlog::info("SEE YOU SPACE COWBOY...");
}

void Spine::cycle() {
  begin_cycle();      // check interrupts, read agent inputs
  cycle_actuation();  // read latest actuation replies, send new commands
  end_cycle();        // output to agent
}

void Spine::simulate(unsigned nb_substeps) {
  while (state_machine_.state() != State::kOver) {
    begin_cycle();
    if (state_machine_.state() == State::kReset) {
      cycle_actuation();  // replies promised but not waited upon yet
      cycle_actuation();  // wait and fill latest_replies_
      cycle_actuation();  // read latest_replies_ and fill observation
      // now the first observation is ready to be read by the agent
    } else if (state_machine_.state() == State::kAct) {
      for (unsigned substep = 0; substep < nb_substeps; ++substep) {
        cycle_actuation();
      }
    }
    end_cycle();
  }
}

void Spine::begin_cycle() {
  if (caught_interrupt_) {
    state_machine_.process_event(Event::kInterrupt);
  } else /* (!caught_interrupt_) */ {
    state_machine_.process_event(Event::kCycleBeginning);
  }
  state_cycle_beginning_ = state_machine_.state();

  // Read input dictionary if applicable
  if (state_machine_.state() == State::kReset) {
    Dictionary config;
    const char* data = agent_interface_.data();
    size_t size = agent_interface_.size();
    config.extend(data, size);
    reset(config);
  } else if (state_machine_.state() == State::kAct) {
    Dictionary& action = dict_("action");
    const char* data = agent_interface_.data();
    size_t size = agent_interface_.size();
    action.update(data, size);
  }
}

void Spine::end_cycle() {
  // Write observation if applicable
  const Dictionary& observation = dict_("observation");
  dict_("time") = observation.get<double>("time");
  if (state_machine_.state() == State::kObserve) {
    size_t size = observation.serialize(ipc_buffer_);
    agent_interface_.write(ipc_buffer_.data(), size);
  }

  state_machine_.process_event(Event::kCycleEnd);
  state_cycle_end_ = state_machine_.state();
}

void Spine::cycle_actuation() {
  try {
    // 1. Observation
    Dictionary& observation = dict_("observation");
    observation::observe_time(observation);
    observation::observe_servos(observation, actuation_.servo_joint_map(),
                                latest_replies_);
    observation::observe_imu(observation, latest_imu_data_);
    // Observers need configuration, so they cannot run at stop
    if (state_machine_.state() != State::kSendStops &&
        state_machine_.state() != State::kShutdown) {
      try {
        observer_pipeline_.run(observation);
      } catch (const ObserverError& e) {
        spdlog::info("Key error from {}: key \"{}\" not found", e.prefix(),
                     e.key());
      }
    }

    // 2. Action
    if (state_machine_.state() == State::kSendStops ||
        state_machine_.state() == State::kShutdown) {
      actuation_.write_stop_commands();
    } else if (state_machine_.state() == State::kAct) {
      Dictionary& action = dict_("action");
      actuation_.write_position_commands(action);
      // TODO(scaron): don't re-send actuation
      // See https://github.com/tasts-robots/vulp/issues/2
      // spdlog::info("[Spine] ok");
    } else {
      // spdlog::warn("[Spine] re-sending previous commands");
    }
  } catch (const std::exception& e) {
    spdlog::error("[Spine] Caught an exception: {}", e.what());
    spdlog::error("[Spine] Sending stop commands...");
    state_machine_.process_event(Event::kInterrupt);
    actuation_.write_stop_commands();
  } catch (...) {
    spdlog::error("[Spine] Caught an unknown exception!");
    spdlog::error("[Spine] Sending stop commands...");
    state_machine_.process_event(Event::kInterrupt);
    actuation_.write_stop_commands();
  }

  // Whatever exceptions were thrown around, we caught them and at this
  // point every actuation command is either a stop or a position one.

  // 3. Wait for the result of the last query and copy it
  if (actuation_output_.valid()) {
    const auto current_values = actuation_output_.get();  // may wait here
    const auto rx_count = current_values.query_result_size;
    latest_replies_.resize(rx_count);
    std::copy(actuation_.replies().begin(),
              actuation_.replies().begin() + rx_count, latest_replies_.begin());
    latest_imu_data_ = actuation_.imu_data();
  }

  // Now we are after the previous cycle (we called actuation_output_.get())
  // and before the next one. This is a good time to break out of loop of
  // communication cycles. Otherwise, the interface may warn that it is waiting
  // for the last actuation cycle to finish.
  if (state_machine_.is_over_after_this_cycle()) {
    spdlog::info("Wrapping up last communication cycle");
    return;
  }

  // 4. Start a new cycle. Results have been copied, so actuation commands and
  // replies are available again to the actuation thread for writing.
  auto promise = std::make_shared<std::promise<actuation::moteus::Output>>();
  actuation_.cycle(actuation_.data(),
                   [promise](const actuation::moteus::Output& output) {
                     // This is called from an arbitrary thread, so we
                     // just set the promise value here.
                     promise->set_value(output);
                   });
  actuation_output_ = promise->get_future();
}

}  // namespace vulp::spine
