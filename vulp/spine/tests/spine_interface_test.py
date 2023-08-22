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

"""
Test the spine interface.
"""

import mmap
import sys
import unittest

import msgpack
import posix_ipc

from vulp.spine.request import Request
from vulp.spine.spine_interface import SpineInterface
from vulp.utils.serialize import serialize

wait_pre_monkeypatch = SpineInterface._wait_for_spine


class TestSpineInterface(unittest.TestCase):
    last_action: dict
    last_config: dict
    shm_name: str
    next_observation: dict
    spine: SpineInterface

    def setUp(self, shm_name="/vulp", size=1024):
        """
        Initialize a new fixture instance.

        Args:
            shm_name: Name of shared memory file.
            size: Size of shared memory file in bytes.
        """
        self.shared_memory = posix_ipc.SharedMemory(
            shm_name, posix_ipc.O_CREAT, size=size, read_only=False
        )
        _mmap = mmap.mmap(
            self.shared_memory.fd,
            self.shared_memory.size,
            flags=mmap.MAP_SHARED,
            prot=mmap.PROT_READ | mmap.PROT_WRITE,
        )
        self.shared_memory.close_fd()
        self._mmap = _mmap
        self._packer = msgpack.Packer(default=serialize, use_bin_type=True)
        self._unpacker = msgpack.Unpacker(raw=False)

        self.last_action = {}
        self.last_config = {}
        self.shm_name = shm_name
        self.next_observation = {
            "servo": {
                "foo": {"position": 0.0},
                "bar": {"position": 1.0},
            }
        }

        def wait_monkeypatch(spine):
            """
            Monkey patch to the interface's `_wait_for_spine` function so that
            TestSpineInterface carries out the tasks expected by the spine.

            Args:
                spine: The spine interface waiting for a free request slot.
            """
            if self.read_request() == Request.kObservation:
                self.write_observation(self.next_observation)
            elif self.read_request() == Request.kAction:
                self.last_action = self.read_dict()
            elif self.read_request() == Request.kStart:
                self.last_config = self.read_dict()
            self.assertEqual(self.read_request(), Request.kNone)

        self.assertTrue(hasattr(SpineInterface, "_wait_for_spine"))
        SpineInterface._wait_for_spine = wait_monkeypatch
        self.spine = SpineInterface(shm_name=shm_name)
        self.write_request(Request.kNone)  # same as Spine constructor

    def tearDown(self) -> None:
        self._mmap.close()
        self.shared_memory.unlink()

    def read_request(self) -> int:
        """
        Get shared memory request.
        """
        self._mmap.seek(0)
        return int.from_bytes(self._mmap.read(4), byteorder=sys.byteorder)

    def write_request(self, request: int) -> None:
        """
        Set shared memory request.
        """
        self._mmap.seek(0)
        self._mmap.write(request.to_bytes(4, byteorder=sys.byteorder))

    def write_observation(self, observation: dict) -> None:
        """
        Set shared memory dictionary to a given observation.

        Args:
            observation: Observation to write to shared memory.
        """
        data = self._packer.pack(observation)
        size = len(data)
        self._mmap.seek(0)
        self._mmap.read(4)
        self._mmap.write(size.to_bytes(4, byteorder=sys.byteorder))
        self._mmap.write(data)
        self.write_request(Request.kNone)

    def read_dict(self) -> dict:
        """
        Read dictionary from the memory shared with the spine interface.

        Returns:
            Dictionary read from shared memory.
        """
        self._mmap.seek(0)
        self._mmap.read(4)  # skip request
        size = int.from_bytes(self._mmap.read(4), byteorder=sys.byteorder)
        data = self._mmap.read(size)
        self._unpacker.feed(data)
        nb_unpacked = 0
        output = {}
        for unpacked in self._unpacker:
            output = unpacked
            nb_unpacked += 1
        self.assertEqual(nb_unpacked, 1)
        self.write_request(Request.kNone)
        return output

    def test_reset(self):
        """
        Spine gets the proper configuration dictionary.
        """
        config = {"foo": {"bar": {"land": 42.0}}}
        self.assertEqual(self.read_request(), Request.kNone)
        self.spine.start(config)
        self.assertEqual(self.read_request(), Request.kStart)
        self.spine.get_observation()  # triggers wait_for_monkeypatch
        self.assertEqual(self.last_config, config)

    def test_get_observation(self):
        """
        Spine switches the request back to none and returns an observation.
        """
        self.assertEqual(self.read_request(), Request.kNone)
        observation = self.spine.get_observation()
        self.assertEqual(self.read_request(), Request.kNone)
        self.assertEqual(observation, self.next_observation)

    def test_set_action(self):
        """
        Step sends an action we deserialize successfully, and returns a new
        observation.
        """
        action = {"servo": {"foo": {"position": 3.0}}}
        self.next_observation["servo"]["foo"]["position"] = 2.0
        self.assertEqual(self.read_request(), Request.kNone)
        self.spine.set_action(action)
        self.assertEqual(self.read_request(), Request.kAction)
        observation = self.spine.get_observation()
        self.assertEqual(self.last_action, action)
        self.assertEqual(observation, self.next_observation)

    def test_wait_times_out(self):
        """
        Disable monkeypatch to check that waiting for the spine (when there is
        no spine) times out.
        """
        SpineInterface._wait_for_spine = wait_pre_monkeypatch
        spine = SpineInterface(shm_name=self.shm_name)
        with self.assertRaises(TimeoutError):
            spine.get_observation()
        with self.assertRaises(TimeoutError):
            spine.set_action({"foo": "bar"})
        with self.assertRaises(TimeoutError):
            spine.stop()


if __name__ == "__main__":
    unittest.main()
