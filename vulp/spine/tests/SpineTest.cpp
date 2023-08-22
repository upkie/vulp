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

#include <palimpsest/Dictionary.h>

#include <cmath>
#include <filesystem>
#include <limits>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "vulp/actuation/MockInterface.h"
#include "vulp/actuation/ServoLayout.h"
#include "vulp/observation/ObserverPipeline.h"
#include "vulp/observation/tests/SchwiftyObserver.h"
#include "vulp/observation/tests/ThrowingObserver.h"
#include "vulp/spine/Spine.h"
#include "vulp/utils/random_string.h"

namespace vulp::spine {

using actuation::MockInterface;
using observation::ObserverPipeline;
using observation::tests::SchwiftyObserver;
using observation::tests::ThrowingObserver;
using palimpsest::Dictionary;
using spine::Spine;

namespace testing {

//! Testing version of the spine class
class Spine : public vulp::spine::Spine {
 public:
  //! Parent constructor.
  using vulp::spine::Spine::Spine;

  //! Get agent interface.
  const AgentInterface& agent_interface() { return agent_interface_; }

  //! Check whether all servo commands are stops.
  bool all_commands_are_stops() {
    for (const auto& command : actuation_.commands()) {
      if (command.mode != actuation::moteus::Mode::kStopped) {
        return false;
      }
    }
    return true;
  }

  //! Get current observation.
  const palimpsest::Dictionary& dict() { return dict_; }

  //! Get current (state of the) state machine.
  const State& state() { return state_machine_.state(); }

  //! Force processing of a given event.
  void force_event(const Event& event) { state_machine_.process_event(event); }

  //! Get servo-bus map.
  const std::map<int, int>& servo_bus_map() {
    return actuation_.servo_bus_map();
  }

  //! Get servo-joint name map.
  const std::map<int, std::string>& servo_joint_map() {
    return actuation_.servo_joint_map();
  }
};

}  // namespace testing

class SpineTest : public ::testing::Test {
 protected:
  //! Initialize spine for a new test
  void SetUp() override {
    params_.cpu = -1;         // no realtime scheduling
    params_.frequency = 400;  // Hz
    params_.shm_name = std::string("/") + utils::random_string();
    params_.shm_size = 1024;

    actuation::ServoLayout layout;
    layout.add_servo(1, 1, "bar");
    layout.add_servo(2, 1, "foo");

    const double dt = 1.0 / params_.frequency;
    actuation_interface_ = std::make_unique<MockInterface>(layout, dt);

    schwifty_observer_ = std::make_unique<SchwiftyObserver>();
    observation_.append_observer(schwifty_observer_);
    spine_ = std::make_unique<testing::Spine>(params_, *actuation_interface_,
                                              observation_);
    int file_descriptor =
        ::shm_open(params_.shm_name.c_str(), O_RDWR | O_CREAT, 0666);
    ASSERT_GE(file_descriptor, 0);
    mmap_ = ::mmap(nullptr, params_.shm_size, PROT_READ | PROT_WRITE,
                   MAP_SHARED, file_descriptor, 0);
    ASSERT_GE(::close(file_descriptor), 0);
  }

  void TearDown() override {
    ASSERT_GE(::munmap(mmap_, params_.shm_size), 0);

    // Check that the destructor unlinks shared memory
    spine_.reset();
    ASSERT_LT(::shm_unlink(params_.shm_name.c_str()), 0);
    ASSERT_EQ(errno, ENOENT);
  }

  Request read_mmap_request() {
    const auto* request = static_cast<const uint32_t*>(mmap_);
    return static_cast<Request>(*request);
  }

  //! Write request to shared memory
  void write_mmap_request(Request request) {
    auto* start = static_cast<uint32_t*>(mmap_);
    *start = static_cast<uint32_t>(request);
  }

  //! Write raw data to shared memory
  void write_mmap_data(const char* data, size_t size) {
    auto* start = static_cast<uint32_t*>(mmap_);
    *(start + 1) = size;
    auto* mmap_data = reinterpret_cast<char*>(start + 2);
    std::memcpy(mmap_data, data, size);
  }

  //! Write a dictionary to shared memory
  void write_mmap_dict(const Dictionary& dict) {
    size_t size = dict.serialize(serialization_buffer_);
    write_mmap_data(serialization_buffer_.data(), size);
  }

  //! Do the transition sequence from stop to idle
  void start_spine() {
    ASSERT_EQ(spine_->state(), State::kSendStops);
    write_mmap_request(Request::kStart);
    for (unsigned cycle = 0; cycle < kNbStopCycles; ++cycle) {
      spine_->cycle();
      ASSERT_EQ(spine_->state(), State::kSendStops);
    }
    spine_->cycle();
    ASSERT_EQ(spine_->state(), State::kIdle);
  }

  //! Valid set of command-line arguments
  Spine::Parameters params_;

  //! Test actuator interface
  std::unique_ptr<MockInterface> actuation_interface_;

  //! Test observer
  std::shared_ptr<SchwiftyObserver> schwifty_observer_;

  //! Test observers
  ObserverPipeline observation_;

  //! Test spine
  std::unique_ptr<testing::Spine> spine_;

  //! Pointer to IPC shared memory
  void* mmap_;

  //! Buffer used to write dictionaries to shared memory
  std::vector<char> serialization_buffer_;
};

TEST_F(SpineTest, ObservationInitializedInConstructor) {
  ASSERT_TRUE(spine_->dict().has("observation"));
  ASSERT_TRUE(spine_->dict().has("time"));
}

TEST_F(SpineTest, InitiallyStopped) {
  ASSERT_EQ(spine_->state(), State::kSendStops);
}

TEST_F(SpineTest, ResetConfiguresAction) {
  Dictionary config;
  spine_->reset(config);
  ASSERT_TRUE(spine_->dict().has("action"));
  const Dictionary& action = spine_->dict()("action");
  ASSERT_TRUE(action.has("brain_time"));
  ASSERT_TRUE(action.has("servo"));
}

TEST_F(SpineTest, NoInitialRequest) {
  ASSERT_EQ(spine_->agent_interface().request(), Request::kNone);
  for (unsigned i = 0u; i < 3u; ++i) {
    spine_->cycle();
    ASSERT_EQ(spine_->agent_interface().request(), Request::kNone);
  }
}

TEST_F(SpineTest, SendStopsOnStartup) {
  ASSERT_EQ(spine_->state(), State::kSendStops);
  write_mmap_request(Request::kStart);
  for (unsigned cycle = 0; cycle < kNbStopCycles; ++cycle) {
    spine_->cycle();
    ASSERT_EQ(spine_->state(), State::kSendStops);
  }
  spine_->cycle();
  ASSERT_EQ(spine_->state(), State::kIdle);
}

TEST_F(SpineTest, SendsStopsOnShutdown) {
  start_spine();
  spine_->force_event(Event::kInterrupt);
  ASSERT_EQ(spine_->state(), State::kShutdown);
  spine_->cycle();
  ASSERT_TRUE(spine_->all_commands_are_stops());
}

TEST_F(SpineTest, CatchObserverErrors) {
  start_spine();
  schwifty_observer_->throw_key_error = true;
  spine_->cycle();
  ASSERT_EQ(spine_->state(), State::kIdle);
}

TEST_F(SpineTest, DontRunObserversWhenStopped) {
  ASSERT_EQ(spine_->state(), State::kSendStops);
  const Dictionary& observation = spine_->dict()("observation");
  spine_->cycle();
  ASSERT_FALSE(observation.has("schwifty"));

  spine_->force_event(Event::kInterrupt);
  spine_->cycle();
  ASSERT_EQ(spine_->state(), State::kShutdown);
  ASSERT_FALSE(observation.has("schwifty"));
}

TEST_F(SpineTest, SetObservationOnIdle) {
  start_spine();
  ASSERT_EQ(spine_->state(), State::kIdle);
  spine_->cycle();
  const Dictionary& observation = spine_->dict()("observation");
  ASSERT_TRUE(observation.get<bool>("schwifty"));
}

TEST_F(SpineTest, WriteObservationOnRequest) {
  start_spine();
  write_mmap_request(Request::kObservation);
  spine_->cycle();
  const Dictionary& observation = spine_->dict()("observation");
  ASSERT_TRUE(observation.get<bool>("schwifty"));
}

TEST_F(SpineTest, ObserverExceptionStopsRun) {
  start_spine();
  schwifty_observer_->throw_exception = true;
  write_mmap_request(Request::kObservation);
  spine_->cycle();
  ASSERT_TRUE(spine_->all_commands_are_stops());
  ASSERT_EQ(spine_->state(), State::kShutdown);
}

TEST_F(SpineTest, ShutdownExecutesFixedNumberOfStopCycles) {
  ASSERT_NE(spine_->state(), State::kShutdown);
  spine_->force_event(Event::kInterrupt);
  ASSERT_EQ(spine_->state(), State::kShutdown);
  spine_->force_event(Event::kCycleEnd);
  for (unsigned cycle = 1; cycle < kNbStopCycles; ++cycle) {
    ASSERT_EQ(spine_->state(), State::kShutdown);
    spine_->cycle();
  }
  ASSERT_EQ(spine_->state(), State::kOver);
}

TEST_F(SpineTest, ResetInitializesAction) {
  Dictionary config;
  spine_->reset(config);
  const Dictionary& spine_dict = spine_->dict();
  ASSERT_TRUE(spine_dict.has("action"));
  ASSERT_TRUE(spine_dict("action").has("servo"));
  ASSERT_TRUE(spine_dict("action")("servo").has("bar"));
  ASSERT_TRUE(spine_dict("action")("servo").has("foo"));
}

TEST_F(SpineTest, SpineReadsAction) {
  start_spine();

  // {"servo": {"bar": {"position": 1.0}}}
  const char data[31] = {'\x81', '\xa5', 's',   'e',    'r',    'v',    'o',
                         '\x81', '\xa3', 'b',   'a',    'r',    '\x81', '\xa8',
                         'p',    'o',    's',   'i',    't',    'i',    'o',
                         'n',    '\xcb', '?',   '\xf0', '\x00', '\x00', '\x00',
                         '\x00', '\x00', '\x00'};
  const size_t size = 31;
  write_mmap_data(data, size);
  const Dictionary& action = spine_->dict()("action");
  write_mmap_request(Request::kAction);
  ASSERT_TRUE(std::isnan(action("servo")("bar").get<double>("position")));
  ASSERT_EQ(spine_->state(), State::kIdle);
  spine_->cycle();
  ASSERT_DOUBLE_EQ(action("servo")("bar")("position"), 1.0);
}

TEST_F(SpineTest, EnteringStopClearsRequest) {
  start_spine();
  ASSERT_EQ(spine_->state(), State::kIdle);
  write_mmap_request(Request::kStop);
  spine_->cycle();
  ASSERT_EQ(spine_->state(), State::kSendStops);
  ASSERT_EQ(read_mmap_request(), Request::kNone);
}

}  // namespace vulp::spine
