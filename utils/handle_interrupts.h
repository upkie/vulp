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

#include <signal.h>

#include <cstddef>

namespace vulp::utils {

namespace internal {

//! Internal interrupt flag.
extern bool interrupt_flag;

//! Internal handler to set \ref interrupt_flag.
void handle_interrupt(int _);

}  // namespace internal

/*! Direct interrupts (e.g. Ctrl-C) to a boolean flag.
 *
 * \return Reference to a boolean flag, which is initially false and becomes
 *     true the first time an interruption is caught.
 */
const bool& handle_interrupts();

}  // namespace vulp::utils
