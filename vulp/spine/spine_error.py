#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# SPDX-License-Identifier: Apache-2.0
# Copyright 2022 Stéphane Caron


class SpineError(Exception):

    """!
    Exception raised when the spine sets an error flag in the request field of
    the shared memory map.
    """
