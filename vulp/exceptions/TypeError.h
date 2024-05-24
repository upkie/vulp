// SPDX-License-Identifier: Apache-2.0
// Copyright 2024 Inria

#pragma once

#include <string>

#include "vulp/exceptions/VulpError.h"

namespace vulp::exceptions {

class TypeError : public VulpError {
 public:
  /*! Create a type error.
   *
   * @param[in] file Source file of the instruction that threw the error.
   * @param[in] line Line of code in that file where the throw originates from.
   * @param[in] message Error message.
   */
  TypeError(const std::string& file, unsigned line, const std::string& message)
      : VulpError(file, line, message) {}

  /*! Copy an existing error, adding to the error message.
   *
   * @param[in] other Existing error.
   * @param[in] extra_message Additional error message.
   */
  TypeError(const TypeError& other, const std::string& extra_message)
      : VulpError(other, extra_message) {}

  //! Empty destructor
  ~TypeError() throw() {}
};

}  // namespace vulp::exceptions
