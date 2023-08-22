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

#include <memory>
#include <string>

#include "gtest/gtest.h"
#include "vulp/spine/AgentInterface.h"
#include "vulp/spine/StateMachine.h"
#include "vulp/utils/random_string.h"

namespace vulp::spine {

using spine::AgentInterface;
using spine::StateMachine;

class StateMachineTest : public ::testing::Test {
 protected:
  //! Prepare state machine for a new test
  void SetUp() override {
    const size_t shm_size = 1 * (1 << 20);
    const std::string shm_name = std::string("/") + utils::random_string();
    agent_interface_ = std::make_unique<AgentInterface>(shm_name, shm_size);
    state_machine_ = std::make_unique<StateMachine>(*agent_interface_);
  }

  //! Get current request
  Request request() { return agent_interface_->request(); }

  //! Get current state
  State state() { return state_machine_->state(); }

  //! Convenience function to set the request directly
  void set_request(const Request& request) {
    agent_interface_->set_request(request);
  }

  //! Test actuator interface
  std::unique_ptr<AgentInterface> agent_interface_;

  //! Test state machine
  std::unique_ptr<StateMachine> state_machine_;
};

TEST_F(StateMachineTest, StartsInStopState) {
  ASSERT_EQ(state(), State::kSendStops);
  ASSERT_EQ(request(), Request::kNone);
}

TEST_F(StateMachineTest, RestopsWork) {
  ASSERT_EQ(state(), State::kSendStops);

  // Requesting a stop from stop state is idempotent
  set_request(Request::kStop);
  state_machine_->process_event(Event::kCycleBeginning);
  ASSERT_EQ(request(), Request::kNone);
  ASSERT_EQ(state(), State::kSendStops);
  state_machine_->process_event(Event::kCycleEnd);
  ASSERT_EQ(request(), Request::kNone);
  ASSERT_EQ(state(), State::kSendStops);
}

TEST_F(StateMachineTest, Startup) {
  ASSERT_EQ(state(), State::kSendStops);

  set_request(Request::kStart);

  // Wait for the required number of stop cycles
  for (unsigned cycle = 0; cycle < kNbStopCycles; ++cycle) {
    state_machine_->process_event(Event::kCycleBeginning);
    ASSERT_EQ(state(), State::kSendStops);
    state_machine_->process_event(Event::kCycleEnd);
    ASSERT_EQ(state(), State::kSendStops);
  }

  // Regular startup sequence
  state_machine_->process_event(Event::kCycleBeginning);
  ASSERT_EQ(state(), State::kReset);
  state_machine_->process_event(Event::kCycleEnd);
  ASSERT_EQ(state(), State::kIdle);
  ASSERT_EQ(request(), Request::kNone);

  // Requesting a second start has no effect
  set_request(Request::kStart);
  state_machine_->process_event(Event::kCycleBeginning);
  ASSERT_EQ(request(), Request::kNone);
  ASSERT_EQ(state(), State::kIdle);
  state_machine_->process_event(Event::kCycleEnd);
  ASSERT_EQ(state(), State::kIdle);
  ASSERT_EQ(request(), Request::kNone);
}

TEST_F(StateMachineTest, ObserveWhenSendingStopsFails) {
  ASSERT_EQ(state(), State::kSendStops);

  set_request(Request::kObservation);
  state_machine_->process_event(Event::kCycleBeginning);
  ASSERT_EQ(state(), State::kSendStops);
  ASSERT_EQ(request(), Request::kError);
}

}  // namespace vulp::spine
