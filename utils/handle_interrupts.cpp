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

#include "vulp/utils/handle_interrupts.h"

namespace vulp::utils {

namespace internal {

bool interrupt_flag = false;

void handle_interrupt(int _) { interrupt_flag = true; }

}  // namespace internal

/*! Redirect interrupts to setting a global interrupt boolean.
 *
 * \return Reference to the interrupt boolean.
 */
const bool& handle_interrupts() {
  struct sigaction handler;
  handler.sa_handler = internal::handle_interrupt;
  sigemptyset(&handler.sa_mask);
  handler.sa_flags = 0;
  sigaction(SIGINT, &handler, NULL);
  return internal::interrupt_flag;
}

}  // namespace vulp::utils
