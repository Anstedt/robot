from machine import Pin
from rp2 import PIO, StateMachine, asm_pio
from time import sleep

@asm_pio(out_init=(rp2.PIO.OUT_HIGH,) * 1, out_shiftdir=PIO.SHIFT_RIGHT, 
 autopull=True, pull_thresh=16)
def paral_prog():
    pull()  
    out(pins, 1)  

paral_sm = StateMachine(0, paral_prog, freq=10000000, out_base=Pin(25))
paral_sm.active(1)


while True:
  sm.write(bytes((1,)))
  time.sleep(0.5)
  sm.write(bytes((0,)))
  time.sleep(0.5)


  
    while True:
    for i in range(500):
        paral_sm.put(i)
#        print(i)
#        sleep(0.01 )




import select
import sys
import machine
import utime

# SPDX-FileCopyrightText: 2021 Jeff Epler, written for Adafruit Industries
#
# SPDX-License-Identifier: MIT
#
# Adapted from the example https://github.com/raspberrypi/pico-examples/tree/master/pio/hello_pio

import time
import board
import rp2pio
import adafruit_pioasm

hello = """
.program hello
loop:
    pull
    out pins, 1
; This program uses a 'jmp' at the end to follow the example.  However,
; in a many cases (including this one!) there is no jmp needed at the end
; and the default "wrap" behavior will automatically return to the "pull"
; instruction at the beginning.
    jmp loop
"""

assembled = adafruit_pioasm.assemble(hello)

sm = rp2pio.StateMachine(
    assembled,
    frequency=2000,
    first_out_pin=board.LED,
)
print("real frequency", sm.frequency)

while True:
    sm.write(bytes((1,)))
    time.sleep(0.5)
    sm.write(bytes((0,)))
    time.sleep(0.5)

; PIO CODE

hello = """
.program hello
loop:
    pull
    out pins, 1
; This program uses a 'jmp' at the end to follow the example. However,
; in a many cases (including this one!) there is no jmp needed at the end
; and the default "wrap" behavior will automatically return to the "pull"
; instruction at the beginning.
    jmp loop
"""
