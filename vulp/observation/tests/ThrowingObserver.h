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

#include "vulp/observation/Observer.h"

namespace vulp::observation::tests {

//! Exception-throwing observer
class ThrowingObserver : public observation::Observer {
 public:
  void read(const palimpsest::Dictionary& observation) override {
    throw std::runtime_error("could not get schwifty");
  }
};

}  // namespace vulp::observation::tests
