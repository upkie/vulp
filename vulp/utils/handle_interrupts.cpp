// Copyright 2022 Stéphane Caron
// SPDX-License-Identifier: Apache-2.0

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
