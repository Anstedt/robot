from rp2 import PIO, StateMachine, asm_pio
from machine import Pin
from time import sleep

# See PICO-Poll for comm. with PI
@asm_pio(set_init=PIO.OUT_HIGH)
def pulse_control():
    label("mainloop")
    pull(noblock)
    mov(x,osr)
    mov(y,osr)
    set(pins, 1) [3] # Turn LED on for 4 1mhz clocks cycles or 4 us delay
    # label("delayloophigh")
    # jmp(y_dec, "delayloophigh")
    set(pins, 0)  # Turn LED off
    # pull()
    # mov(x,osr)
    # mov(y,osr)
    mov(y,osr)
    label("delaylooplow")
    pull(noblock)    # Keep getting the latest value or x if no new values
    mov(x,osr)       # Remember mov() is right to left
    jmp(y_dec, "delaylooplow")
    jmp("mainloop") # Jump back to the beginning

sm = StateMachine(1, pulse_control, freq=1000000, set_base=Pin(16))  # Instantiate SM1, 2000 Hz, LED on pin 3
sm.active(1)                                                 # Start State Machine 1

while True:
  sm.put(1)
  print("1 sleep 10 for time testing")
  sleep(15)
  sm.put(20)
  print("20 sleep 5")
  sleep(15)
  sm.put(100)
  print("100 sleep 5")
  sleep(5)
  sm.put(20000)
  print("20000 sleep 5")
  sleep(5)

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
