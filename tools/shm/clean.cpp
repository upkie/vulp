// Copyright 2022 Stéphane Caron
// SPDX-License-Identifier: Apache-2.0

#include <fcntl.h>
#include <spdlog/spdlog.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

namespace tools::shm {

//! Command-line arguments.
class CommandLineArguments {
 public:
  /*! Read command line arguments.
   *
   * \param[in] args List of command-line arguments.
   */
  explicit CommandLineArguments(const std::vector<std::string>& args) {
    for (size_t i = 0; i < args.size(); i++) {
      const auto& arg = args[i];
      if (arg == "-h" || arg == "--help") {
        help = true;
      } else if (arg[0] != '-') {
        filename = args.at(i);
      } else {
        spdlog::error("Unknown argument: {}", arg);
        error = true;
      }
    }
  }

  /*! Show help message
   *
   * \param[in] name Binary name from argv[0].
   */
  inline void print_usage(const char* name) noexcept {
    std::cout << "Usage: " << name << " <filename> [options]\n";
    std::cout << "\n";
    std::cout << "Optional arguments:\n\n";
    std::cout << "-h, --help\n"
              << "    Print this help and exit.\n";
    std::cout << "-s, --show\n"
              << "    Show the Bullet GUI.\n";
    std::cout << "\n";
  }

 public:
  //! Error flag
  bool error = false;

  //! Help flag
  bool help = false;

  //! Name of shared memory file
  std::string filename = "/vulp";
};

int main(const CommandLineArguments& args) {
  const char* name = args.filename.c_str();
  int file_descriptor = ::shm_open(name, O_RDWR, 0666);
  if (file_descriptor < 0) {
    if (errno == ENOENT) {
      spdlog::warn("File {} did not exist, nothing to clean", name);
    } else if (errno == EINVAL) {
      spdlog::error("errno == EINVAL: The name argument was invalid.");
    } else {
      spdlog::error("Error opening file; errno is {}", errno);
    }
    return EXIT_FAILURE;
  }
  if (::shm_unlink(name) < 0) {
    spdlog::error("Failed to unlink shared memory; errno is {}", errno);
  }
  return EXIT_SUCCESS;
}

}  // namespace tools::shm

int main(int argc, char** argv) {
  tools::shm::CommandLineArguments args({argv + 1, argv + argc});
  if (args.error) {
    return EXIT_FAILURE;
  } else if (args.help) {
    args.print_usage(argv[0]);
    return EXIT_SUCCESS;
  }
  return tools::shm::main(args);
}
