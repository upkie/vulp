// SPDX-License-Identifier: Apache-2.0
// Copyright 2022 Stéphane Caron

#pragma once

#include <palimpsest/Dictionary.h>
#include <spdlog/spdlog.h>

#include <Eigen/Geometry>
#include <limits>
#include <map>
#include <string>

#include "vulp/actuation/ImuData.h"
#include "vulp/actuation/Interface.h"
#include "vulp/actuation/moteus/protocol.h"

namespace vulp::actuation {

class MockInterface : public Interface {
 public:
  /*! Create mock actuator interface.
   *
   * \param[in] params Interface parameters.
   */
  MockInterface(const ServoLayout& layout, const double dt);

  //! Default destructor
  ~MockInterface() = default;

  /*! Reset interface.
   *
   * \param[in] config Additional configuration dictionary.
   */
  void reset(const Dictionary& config) override;

  /*! Write actuation-interface observations to dictionary.
   *
   * \param[out] observation Dictionary to write ot.
   */
  void observe(Dictionary& observation) const override;

  /*! Simulate a new communication cycle.
   *
   * \param data Buffer to read commands from and write replies to.
   * \param callback Function to call when the cycle is over.
   *
   * The callback will be invoked from an arbitrary thread when the
   * communication cycle has completed. All memory pointed to by @p data must
   * remain valid until the callback is invoked.
   */
  void cycle(const moteus::Data& data,
             std::function<void(const moteus::Output&)> callback) final;

 private:
  //! Spine timestep in [s].
  const double dt_;

  //! Mock servo replies, laid out as a servo_id -> query_result map
  std::map<int, moteus::QueryResult> query_results_;

  //! Mock IMU data
  ImuData imu_data_;
};

}  // namespace vulp::actuation
