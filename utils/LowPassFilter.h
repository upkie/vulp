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
 *     LowPassFilter classes of lipm_walking_controller
 *     Copyright (c) 2018-2019, CNRS-UM LIRMM
 *     License: BSD-2-Clause
 */

#pragma once

#include <Eigen/Dense>
#include <algorithm>

namespace vulp::utils {

/*! Base class for low-pass filters.
 *
 * \note This base class has all the functionality except for the reset
 * function, which differs between primitive-type and vectorizable
 * specializations.
 *
 * If we need more advanced filters, there is e.g. a collection of digital
 * filters in https://github.com/vsamy/DiFipp (BSD 2-clause + MIT).
 */
template <typename T>
class LowPassFilterBase {
 public:
  /*! Constructor with update constant.
   *
   * \param alpha Update constant of the filter.
   *
   * \note Reset needs to be handled by derived classes.
   */
  explicit LowPassFilterBase(double alpha) { setAlpha(alpha); }

  /*! Constructor with time constants.
   *
   * \param period Cutoff period in [s].
   * \param dt Sampling period in [s].
   *
   * \note Reset needs to be handled by derived classes.
   */
  LowPassFilterBase(double period, double dt) { setCutoffPeriod(period, dt); }

  //! Reset filter to zero.
  virtual void reset() = 0;

  /*! Reset filter to a given value.
   *
   * \param value New value.
   */
  void reset(T value) { output_ = value; }

  /*! Set update constant \f$\alpha\f$ of the filter directly.
   *
   * \param alpha New update constant (unitless).
   */
  void setAlpha(double alpha) { alpha_ = alpha; }

  /*! Set cutoff period.
   *
   * \param period Cutoff period in [s].
   * \param dt Sampling period in [s].
   */
  void setCutoffPeriod(double period, double dt) {
    period = std::max(period, 2 * dt);  // Nyquist–Shannon sampling theorem
    alpha_ = dt / period;
  }

  /*! Step the filter with a new input value.
   *
   * \param value New input.
   */
  void step(const T& value) { output_ += alpha_ * (value - output_); }

  //! Get update constant of the filter.
  double alpha() const { return alpha_; }

  //! Get filtered output.
  const T& get() const { return output_; }

 private:
  //! Filtered output
  T output_;

  //! Update constant of the filter (unitless)
  double alpha_;
};

//! Low-pass filter on Eigen vectorizable types.
template <typename T>
class LowPassFilter : public LowPassFilterBase<T> {
 public:
  // In case T is fixed-size vectorizable. See:
  // https://eigen.tuxfamily.org/dox/group__TopicStructHavingEigenMembers.html
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  /*! Constructor with update constant.
   *
   * \param alpha Update constant of the filter.
   */
  explicit LowPassFilter(double alpha) : LowPassFilterBase<T>(alpha) {
    reset();
  }

  /*! Constructor with time constants.
   *
   * \param period Cutoff period in [s].
   * \param dt Sampling period in [s].
   */
  LowPassFilter(double period, double dt) : LowPassFilterBase<T>(period, dt) {
    reset();
  }

  //! Reset filter to T's zero.
  void reset() { LowPassFilterBase<T>::reset(T::Zero()); }
};

//! Low-pass filter on scalar values.
template <>
class LowPassFilter<double> : public LowPassFilterBase<double> {
 public:
  /*! Constructor with update constant.
   *
   * \param alpha Update constant of the filter.
   */
  explicit LowPassFilter(double alpha) : LowPassFilterBase(alpha) { reset(); }

  /*! Constructor with time constants.
   *
   * \param period Cutoff period in [s].
   * \param dt Sampling period in [s].
   */
  LowPassFilter(double period, double dt) : LowPassFilterBase(period, dt) {
    reset();
  }

  //! Reset filter to zero.
  void reset() { LowPassFilterBase::reset(0.); }

  //! Reset filter to a given value
  void reset(double value) { LowPassFilterBase::reset(value); }
};

}  // namespace vulp::utils
