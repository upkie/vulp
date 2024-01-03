#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# SPDX-License-Identifier: Apache-2.0
# Copyright 2022 Stéphane Caron

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
