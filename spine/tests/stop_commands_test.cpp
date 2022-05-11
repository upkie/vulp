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

#include "vulp/spine/stop_commands.h"

#include <map>
#include <string>
#include <vector>

#include "gtest/gtest.h"

namespace vulp::spine {

TEST(WriteStopCommands, SetAllCommandsToStop) {
  std::vector<actuation::moteus::ServoCommand> commands;
  commands.push_back({});
  commands.back().id = 0;
  commands.push_back({});
  commands.back().id = 12;

  write_stop_commands(commands);
  for (const auto& command : commands) {
    ASSERT_EQ(command.mode, actuation::moteus::Mode::kStopped);
  }
}

}  // namespace vulp::spine
