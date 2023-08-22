#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# Copyright 2022 Stéphane Caron
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import logging
from os import path

import IPython
from vulp.spine import Request, SpineInterface

if __name__ == "__main__":
    agent_dir = path.dirname(__file__)
    spine = SpineInterface()
    try:
        spine.start(config={})
    except TimeoutError:
        logging.error("Configuration timed out")
    if spine._read_request() == Request.kNone:  # use Request somehow (^_^)
        logging.info("Woohoo, spine is fast today!")

    print("")
    print("Usage:")
    print("- spine.get_observation()")
    print('- spine.set_action({"foo": 42.0})')
    print("")

    IPython.embed()
    spine.stop()
