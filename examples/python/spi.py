#!/usr/bin/env python

# Author: Henry Bruce <henry.bruce@intel.com>
# Copyright (c) 2016 Intel Corporation.
#
# SPDX-License-Identifier: MIT

 # Example usage: Display set of patterns on MAX7219 repeately.
 #                Press Ctrl+C to exit

import mraa
import time
import sys



pats = [0xaa01, 0x5502, 0xaa03, 0x5504, 0xaa05, 0x5506, 0xaa07, 0x5508 ]
pats_inv = [0x5501, 0xaa02, 0x5503, 0xaa04, 0x5505, 0xaa06, 0x5507, 0xaa08 ]
pats_clear = [0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008 ]


spi_num = 0 if len(sys.argv) < 2 else int(sys.argv[1])
spi_speed = 100000 if len(sys.argv) < 3 else int(sys.argv[2])


print("SPI BUS:",spi_num,"\tFreqency:",spi_speed,"\n")

# initialise SPI
dev = mraa.Spi(spi_num)
time.sleep(0.05)

# set speed
dev.frequency(spi_speed)
time.sleep(0.05)

# set bits per mode
dev.bitPerWord(16)
time.sleep(0.05)

# do not decode bits 
txbuf = 0x0009 
dev.writeWord(txbuf) # 0x9 is for Decode mode, an 8x8 LEDs should be set to 0x00
time.sleep(0.05)

# brightness of LEDs
txbuf = 0x050A
dev.writeWord(txbuf) # 0xA is for intersity, could be set from 0x0 to 0xF and 0xF is the brightest.
time.sleep(0.05)

# show all scan lines     
txbuf = 0x070B
dev.writeWord(txbuf) # 0xB is for Scan, means how many EEPROM you want to open. 
time.sleep(0.05)

# set display on 
txbuf = 0x010C
dev.writeWord(txbuf) # 0xC: set to 0x0 for shutdown, 0x1 for open.
time.sleep(0.05) 

# testmode off 
txbuf = 0x000F
dev.writeWord(txbuf) # 0xF is for test mode, if set to 0x0 every LED will set bright.
time.sleep(0.05)


while True:
    # set display pattern
    for pat in pats:
        dev.writeWord(pat)
        time.sleep(0.05)
    time.sleep(1)

    #set inverted display pattern
    for pat_inv in pats_inv:
        dev.writeWord(pat_inv)
        time.sleep(0.05)
    time.sleep(1)

    #clear the LED's 
    for pat_clear in pats_clear:
        dev.writeWord(pat_clear)
    time.sleep(1)
    
    #cycle through all LED's 
    for i in range(1,9):
        for j in range(0,8):
            dev.writeWord(i  + (1 << (j+8)))
            time.sleep(0.05)
        dev.writeWord(i << 8)