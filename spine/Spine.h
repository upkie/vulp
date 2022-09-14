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

#pragma once

#include <mpacklog/Logger.h>

#include <algorithm>
#include <future>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "vulp/actuation/ImuData.h"
#include "vulp/actuation/Interface.h"
#include "vulp/observation/ObserverPipeline.h"
#include "vulp/observation/observe_imu.h"
#include "vulp/observation/observe_servos.h"
#include "vulp/observation/observe_time.h"
#include "vulp/spine/AgentInterface.h"
#include "vulp/spine/StateMachine.h"
#include "vulp/utils/SynchronousClock.h"
#include "vulp/utils/handle_interrupts.h"
#include "vulp/utils/realtime.h"

namespace vulp::spine {

constexpr size_t kMebibytes = 1 << 20;

/*! Loop transmitting actions to the actuation and observations to the agent.
 *
 * The spine acts as the intermediary between the actuation interface (e.g.
 * moteus servos connected to the CAN-FD bus) and an agent communicating over
 * shared memory (the agent interface). It packs observations to the agent from
 * actuation replies and commands to the actuation from agent actions.
 *
 * The spine processes requests at the beginning and end of each control cycle
 * according to its StateMachine. The overall specification of the state
 * machine is summarized in the following diagram:
 *
 * \image html state-machine.png
 * \image latex state-machine.eps
 *
 * See StateMachine for more details.
 */
class Spine {
 public:
  //! Spine parameters.
  struct Parameters {
    //! CPUID for the spine thread (-1 to disable realtime).
    int cpu = -1;

    //! Frequency of the spine loop in [Hz].
    unsigned frequency = 1000u;

    //! Path to output log file
    std::string log_path = "/dev/null";

    //! Name of the shared memory object for inter-process communication
    std::string shm_name = "/vulp";

    //! Size of the shared memory object in bytes
    size_t shm_size = 1 * kMebibytes;
  };

  /*! Initialize spine.
   *
   * \param[in] params Spine parameters.
   * \param[in, out] interface Interface to actuators.
   * \param[in, out] observers Pipeline of observers to run, in that order, at
   *     each cycle.
   */
  Spine(const Parameters& params, actuation::Interface& interface,
        observation::ObserverPipeline& observers);

  /*! Reset the spine with a new configuration.
   *
   * \param[in] config New global configuration dictionary, used to derive the
   *     servo layout (which servo is on which bus, corresponds to which joint)
   *     and forwarded to other components (e.g. observers).
   */
  void reset(const palimpsest::Dictionary& config);

  /*! Run the spine loop until termination.
   *
   * Each iteration of the loop runs observers, computes the action and cycles
   * the actuation interface. Additionally, this function collects debug values
   * and logs everything.
   *
   * \note The spine will catch keyboard interrupts once this function is
   * called.
   */
  void run();

  //! Spin one cycle of the spine loop.
  void cycle();

  /*! Alternative to \ref run where the actuation interface is cycled a fixed
   * number of times, and communication cycles are not frequency-regulated.
   *
   * \param[in] nb_substeps Number of actuation cycles per action.
   *
   * Thus function assumes the agent alternates acting and observing.
   * Simulation steps are triggered at startup (to construct the initial
   * observation) and at each action request.
   *
   * \note As its name suggests, do not use this function on a real robot.
   */
  void simulate(unsigned nb_substeps);

 private:
  //! Begin cycle: check interrupts and read agent inputs
  void begin_cycle();

  /*! Spin one cycle of communications with the actuation interface.
   *
   * A cycle consists in:
   *
   * 1. Run the observer pipeline over the latest reply
   * 2. Prepare servo commands
   * 3. Wait to receive the reply from the previous cycle over the interface
   * 4. If applicable, start the next cycle:
   */
  void cycle_actuation();

  //! End cycle: write agent outputs, apply state machine transition
  void end_cycle();

  //! Log internal dictionary
  void log_dict();

 protected:
  //! Frequency of the spine loop in [Hz].
  const unsigned frequency_;

  /*! Interface that communicates with actuators.
   *
   * The actuation interface communicates over the CAN-FD bus on real robots.
   * Otherwise, it can be for instance a mock or a simulator interface.
   */
  actuation::Interface& actuation_;

  //! Shared memory mapping for inter-process communication
  AgentInterface agent_interface_;

  //! Future used to wait for moteus replies
  std::future<actuation::moteus::Output> actuation_output_;

  //! Latest servo replies. They are copied and thread-safe.
  std::vector<actuation::moteus::ServoReply> latest_replies_;

  //! Latest IMU data. It is copied and thread-safe.
  actuation::ImuData latest_imu_data_;

  //! All data from observation and control goes to this dictionary
  palimpsest::Dictionary dict_;

  //! Pipeline of observers, executed in that order
  observation::ObserverPipeline observer_pipeline_;

  //! Logger for the dictionary \ref dict_ produced at each cycle
  mpacklog::Logger logger_;

  //! Buffer used to serialize/deserialize dictionaries in IPC.
  std::vector<char> ipc_buffer_;

  //! Boolean flag that becomes true when an interruption is caught.
  const bool& caught_interrupt_;

  //! Internal state machine.
  StateMachine state_machine_;

  //! State after the last Event::kCycleBeginning
  State state_cycle_beginning_;

  //! State after the last Event::kCycleEnd
  State state_cycle_end_;
};

}  // namespace vulp::spine
