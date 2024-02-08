#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# SPDX-License-Identifier: Apache-2.0
# Copyright 2022 Stéphane Caron
# Copyright 2024 Inria

import logging
import mmap
import sys
import time
from multiprocessing.shared_memory import SharedMemory
from time import perf_counter_ns
from typing import Optional

import msgpack

from vulp.utils import serialize

from .exceptions import PerformanceIssue, SpineError
from .request import Request


def wait_for_shared_memory(
    shm_name: str,
    retries: int,
) -> Optional[SharedMemory]:
    """!
    Connect to the spine shared memory.

    @param shm_name Name of the shared memory object.
    @param retries Number of times to try opening the shared-memory file.
    """
    for trial in range(retries):
        if trial > 0:
            logging.info(
                f"Waiting for spine {shm_name} to start "
                f"(trial {trial} / {retries})..."
            )
            time.sleep(1.0)
        try:
            return SharedMemory(shm_name, size=0, create=False)
        except FileNotFoundError:
            pass
    return None


class SpineInterface:

    """!
    Interface to interact with a spine from a Python agent.
    """

    _mmap: mmap.mmap

    def __init__(
        self,
        shm_name: str = "/vulp",
        retries: int = 1,
        perf_checks: bool = True,
    ):
        """!
        Connect to the spine shared memory.

        @param shm_name Name of the shared memory object.
        @param retries Number of times to try opening the shared-memory file.
        @param perf_checks If true, run performance checks after construction.
        """
        shared_memory = wait_for_shared_memory(shm_name, retries)
        if shared_memory is None:
            raise RuntimeError(
                f"spine {shm_name} did not respond after {retries} attempts"
            )
        try:
            _mmap = mmap.mmap(
                shared_memory.fd,
                shared_memory.size,
                flags=mmap.MAP_SHARED,
                prot=mmap.PROT_READ | mmap.PROT_WRITE,
            )
        except ValueError as exn:
            if "empty file" in str(exn):
                raise RuntimeError("spine is not running") from exn
            raise exn
        shared_memory.close_fd()
        self._mmap = _mmap
        self._packer = msgpack.Packer(default=serialize, use_bin_type=True)
        self._unpacker = msgpack.Unpacker(raw=False)
        self._stop_waiting = set([Request.kNone, Request.kError])
        if perf_checks:
            self.__perf_checks()

    def __perf_checks(self):
        packer_cls = str(msgpack.Packer)
        unpacker_cls = str(msgpack.Unpacker)
        if "fallback" in packer_cls or "fallback" in unpacker_cls:
            # See https://github.com/upkie/vulp/issues/74
            raise PerformanceIssue("msgpack is running in pure Python")

    def __del__(self):
        """!
        Close memory mapping.

        Note that the spine process will unlink the shared memory object, so we
        don't unlink it here.
        """
        if hasattr(self, "_mmap"):  # handle ctor exceptions
            self._mmap.close()

    def get_observation(self) -> dict:
        """!
        Ask the spine to write the latest observation to shared memory.

        @returns Observation dictionary.

        @note In simulation, the first observation after a reset was collected
        before that reset. Use @ref get_first_observation in that case to skip
        to the first post-reset observation.
        """
        self._wait_for_spine()
        self._write_request(Request.kObservation)
        self._wait_for_spine()
        observation = self._read_dict()
        return observation

    def get_first_observation(self) -> dict:
        """!
        Get first observation after a reset.

        @returns Observation dictionary.
        """
        self.get_observation()  # pre-reset observation, skipped
        return self.get_observation()

    def set_action(self, action: dict) -> None:
        self._wait_for_spine()
        self._write_dict(action)
        self._write_request(Request.kAction)

    def start(self, config: dict) -> None:
        """!
        Reset the spine to a new configuration.

        @param config Configuration dictionary.
        """
        self._wait_for_spine()
        self._write_dict(config)
        self._write_request(Request.kStart)

    def stop(self) -> None:
        """!
        Tell the spine to stop all actuators.
        """
        self._wait_for_spine()
        self._write_request(Request.kStop)

    def _read_request(self) -> int:
        """!
        Read current request from shared memory.
        """
        self._mmap.seek(0)
        return int.from_bytes(self._mmap.read(4), byteorder=sys.byteorder)

    def _read_dict(self) -> dict:
        """!
        Read dictionary from shared memory.

        @returns Observation dictionary.
        """
        assert self._read_request() == Request.kNone
        self._mmap.seek(0)
        self._mmap.read(4)  # skip request field
        size = int.from_bytes(self._mmap.read(4), byteorder=sys.byteorder)
        data = self._mmap.read(size)
        self._unpacker.feed(data)
        unpacked = 0
        last_dict = {}
        for observation in self._unpacker:
            last_dict = observation
            unpacked += 1
        assert unpacked == 1
        return last_dict

    def _wait_for_spine(self, timeout_ns: int = 100000000) -> None:
        """!
        Wait for the spine to signal itself as available, which it does by
        setting the current request to none in shared memory.

        @param timeout_ns Don't wait for more than this duration in
            nanoseconds.
        """
        stop = perf_counter_ns() + timeout_ns
        while self._read_request() not in self._stop_waiting:  # sets are fast
            # Fun fact: `not in set` is 3-4x faster than `!=` on the raspi
            # perf_counter_ns clocks ~1 us on the raspi
            if perf_counter_ns() > stop:
                raise TimeoutError(
                    "Spine did not process request within "
                    f"{timeout_ns / 1e6:.1f} ms, is it stopped?"
                )
        if self._read_request() == Request.kError:
            # TODO(scaron): cover this case by a unit test
            self._write_request(Request.kNone)
            raise SpineError("Invalid request, is the spine started?")

    def _write_request(self, request: int) -> None:
        """!
        Set request in shared memory.
        """
        self._mmap.seek(0)
        self._mmap.write(request.to_bytes(4, byteorder=sys.byteorder))

    def _write_dict(self, dictionary: dict) -> None:
        """!
        Set the shared memory to a given dictionary.

        @param dictionary Dictionary to pack and write.
        """
        assert self._read_request() == Request.kNone
        data = self._packer.pack(dictionary)
        size = len(data)
        self._mmap.seek(0)
        self._mmap.read(4)  # skip request field
        self._mmap.write(size.to_bytes(4, byteorder=sys.byteorder))
        self._mmap.write(data)
