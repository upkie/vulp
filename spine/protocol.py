#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# Copyright 2022 Stéphane Caron

from enum import IntEnum


class Request(IntEnum):

    kNone = 0  # no active request
    kObservation = 1
    kAction = 2
    kStart = 3
    kStop = 4
    kError = 5  # last request was invalid
