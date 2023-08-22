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

#include <palimpsest/Dictionary.h>

#include <cmath>
#include <filesystem>
#include <limits>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "vulp/spine/AgentInterface.h"
#include "vulp/utils/random_string.h"

namespace vulp::spine {

class AgentInterfaceTest : public ::testing::Test {
 protected:
  void SetUp() override {
    agent_interface_ =
        std::make_unique<AgentInterface>("/" + utils::random_string(), 1024);
  }

  void TearDown() override {}

  std::unique_ptr<AgentInterface> agent_interface_;
};

TEST_F(AgentInterfaceTest, GetSetRequest) {
  agent_interface_->set_request(Request::kObservation);
  ASSERT_EQ(agent_interface_->request(), Request::kObservation);
  agent_interface_->set_request(Request::kAction);
  ASSERT_EQ(agent_interface_->request(), Request::kAction);
}

TEST_F(AgentInterfaceTest, ReadWriteData) {
  char data[] = "foobar\0";
  const size_t size = 7;
  agent_interface_->write(data, size);
  ASSERT_STREQ(agent_interface_->data(), data);
  ASSERT_EQ(agent_interface_->size(), size);
}

}  // namespace vulp::spine
