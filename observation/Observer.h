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

#include <palimpsest/Dictionary.h>

#include <string>

namespace vulp::observation {

using palimpsest::Dictionary;

//! Base class for observers.
class Observer {
 public:
  //! Destructor is virtual to deallocate lists of observers properly.
  virtual ~Observer() {}

  //! Prefix of outputs in the observation dictionary.
  virtual inline std::string prefix() const noexcept {
    return "unknown_source";
  }

  /*! Reset observer.
   *
   * \param[in] config Configuration dictionary.
   */
  virtual void reset(const Dictionary& config) {}

  /*! Read inputs from other observations.
   *
   * \param[in] observation Dictionary to read other observations from.
   *
   * \note The base class reads nothing. We put an empty function here rather
   * than making the class abstract to be able to instantiate vectors of it.
   */
  virtual void read(const Dictionary& observation) {}

  /*! Write outputs, called if reading was successful.
   *
   * \param[out] observation Dictionary to write observations to.
   *
   * \note The base class writes nothing. We put an empty function here rather
   * than making the class abstract to be able to instantiate vectors of it.
   */
  virtual void write(Dictionary& observation) {}
};

}  // namespace vulp::observation
