// Copyright 2022 Stéphane Caron
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <palimpsest/Dictionary.h>

#include <map>
#include <string>
#include <vector>

#include "vulp/actuation/ImuData.h"

namespace vulp::observation {

/*! Observe the orientation of the IMU and its derivatives.
 *
 * \param[out] observation Observation dictionary.
 * \param[in] imu_data IMU data filtered by the pi3hat.
 */
void observe_imu(palimpsest::Dictionary& observation,
                 const actuation::ImuData& imu_data);

}  // namespace vulp::observation
