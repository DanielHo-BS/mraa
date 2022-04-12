#!/usr/bin/env python

# Author: Brendan Le Foll <brendan.le.foll@intel.com>
# Copyright (c) 2014 Intel Corporation.
#
# SPDX-License-Identifier: MIT
#
# Example Usage: Triggers ISR upon GPIO state change

import mraa
import time
import sys

class Counter:
    count = 0

c = Counter()

# inside a python interrupt you cannot use 'basic' types so you'll need to use
# objects
def isr_routine(gpio):
    c.count += 1
    print("pin " + repr(gpio.getPin(True)) + " = " + repr(gpio.read()))
    print("count = ", c.count)

# GPIO
pin = 5 if len(sys.argv) < 2 else int(sys.argv[1])

try:
    # initialise GPIO
    x = mraa.Gpio(pin)

    print("Starting ISR for pin " + repr(pin))

    # set direction and edge types for interrupt
    x.dir(mraa.DIR_IN)
    x.isr(mraa.EDGE_BOTH, isr_routine, x)

    # wait until ENTER is pressed
    var = input("Press ENTER to stop\n")
    x.isrExit()
except ValueError as e:
    print(e)