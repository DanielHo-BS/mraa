#!/usr/bin/env python

# Author: Henry Bruce <henry.bruce@intel.com>
# Copyright (c) 2016 Intel Corporation.
#
# SPDX-License-Identifier: MIT

# Example Usage: Read from MCP3004 ADC pin 0 in single ended mode

import mraa
import time
import sys

spi_num = 0 if len(sys.argv) < 2 else int(sys.argv[1])
spi_speed = 100000 if len(sys.argv) < 3 else int(sys.argv[2])
spi_bit = 16 if len(sys.argv) < 3 else int(sys.argv[3])

print("SPI BUS:",spi_num,"\tFreqency:",spi_speed,"\tBit per word:",spi_bit)

# initialise SPI
dev = mraa.Spi(spi_num)
time.sleep(0.05)

# set speed
dev.frequency(spi_speed)

# set bits per mode
dev.bitPerWord(spi_bit)

# Take MAX7219 chip with LED Matrix as example.
# We are writing 0x00 to address 0x09 on the chip to brigh the LED Matrix. 

# prepare data to send
txbuf = bytearray(2)
print("Enter the 2 byte data to sned without \"0x\" ")
txbuf[0] = int(input("First Byte: "),16)
txbuf[1] = int(input("Second Byte: "),16)

# send data through SPI
rxbuf = dev.write(txbuf)
print(rxbuf)