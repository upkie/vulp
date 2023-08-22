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

from enum import IntEnum


class Request(IntEnum):

    """!
    Flag set by the agent to request an operation from the spine.

    Once the spine has processed the request, it resets the flag to @ref kNone
    in shared memory.
    """

    kNone = 0  # no active request
    kObservation = 1
    kAction = 2
    kStart = 3
    kStop = 4
    kError = 5  # last request was invalid
