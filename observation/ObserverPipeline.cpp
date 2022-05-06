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

#include "vulp/observation/ObserverPipeline.h"

#include <palimpsest/KeyError.h>

#include "vulp/observation/ObserverError.h"

namespace vulp::observation {

using palimpsest::KeyError;

void ObserverPipeline::reset(const Dictionary& config) {
  for (auto observer : observers_) {
    observer->reset(config);
  }
}

void ObserverPipeline::run(Dictionary& observation) {
  for (auto source : sources_) {
    source->write(observation);
  }
  for (auto observer : observers_) {
    try {
      observer->read(observation);
      observer->write(observation);
    } catch (const KeyError& e) {
      throw ObserverError(observer->prefix(), e.key());
    } catch (const std::exception& e) {
      spdlog::error(
          "[ObserverPipeline::run] Observer {} threw an exception: {}",
          observer->prefix(), e.what());
      throw;
    }
  }
}

}  // namespace vulp::observation
