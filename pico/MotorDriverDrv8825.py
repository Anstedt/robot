from rp2 import PIO, StateMachine, asm_pio
from machine import Pin
from time import sleep

# See PICO-Poll for comm. with PI
@asm_pio(set_init=PIO.OUT_LOW)
def pulse_control():
    label("mainloop")
    pull(noblock)
    mov(x,osr)
    mov(y,osr)
    set(pins, 1)    [31]    # Turn LED on
    label("delayloophigh")
    jmp(y_dec, "delayloophigh")
    set(pins, 0)    [31]    # Turn LED off
    # pull()
    # mov(x,osr)
    # mov(y,osr)
    mov(y,osr)
    label("delaylooplow")
    jmp(y_dec, "delaylooplow")
    jmp("mainloop") [31]    # Jump back to the beginning

sm = StateMachine(1, pulse_control, freq=2000, set_base=Pin(25))  # Instantiate SM1, 2000 Hz, LED on pin 3
sm.active(1)                                                 # Start State Machine 1

while True:
  sm.put(100)
  print("100")
  sleep(2)
  sm.put(2001)
  print("2001")
  sleep(1)
  sm.put(1001)
  print("1001")
  sleep(1)
  sm.put(2001)
  print("101")
  sleep(10)

import select
import sys
import machine
import utime

# # Set up the poll object
# poll_obj = select.poll()
# poll_obj.register(sys.stdin, select.POLLIN)

# # Loop indefinitely
# while True:
#   # Wait for input on stdin
#   poll_results = poll_obj.poll(1) # the '1' is how long it will wait for message before looping again (in microseconds)
#   if poll_results:
#     # Read the data from stdin (read data coming from PC)
#     data = sys.stdin.readline().strip()
#     # Write the data to the input file (print sends data back over USB to the PC)
#     print("received data: ", data)
#   else:
#     # do something if no message received (like feed a watchdog timer)
#     continue
