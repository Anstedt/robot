from rp2 import PIO, StateMachine, asm_pio
from machine import Pin
from time import sleep

# See PICO-Poll for comm. with PI
@asm_pio(set_init=PIO.OUT_HIGH)
def pulse_control():
    label("mainloop")
    pull(noblock)
    mov(x,osr)
    set(pins, 1) [3] # Turn pin on for 4 1mhz clocks cycles or 4 us delay
    set(pins, 0)     # Turn pin off
    mov(y,osr)       # Now get the low delay time
    label("delaylooplow")
    pull(noblock)    # Keep getting the latest value or x if no new values
    mov(x,osr)       # Remember mov() is right to left
    jmp(y_dec, "delaylooplow")
    jmp("mainloop") # Jump back to the beginning

sm = StateMachine(1, pulse_control, freq=1000000, set_base=Pin(16))  # Instantiate SM1, 2000 Hz, LED on pin 3
sm.active(1)                                                 # Start State Machine 1

def pps_to_delay(val):
  return(int(((1/val)*1000000)/3) - 4)

while True:
  pps = 6400
  delaytime = pps_to_delay(pps)
  sm.put(delaytime)
  print(delaytime, "us looking for", pps, "Hz")
  sleep(10)
  
  pps = 1600
  delaytime = pps_to_delay(pps)
  sm.put(delaytime)
  print(delaytime, "us looking for", pps, "Hz")
  sleep(10)
  
  pps = 800
  delaytime = pps_to_delay(pps)
  sm.put(delaytime)
  print(delaytime, "us looking for", pps, "Hz")
  sleep(10)

  pps = 400
  delaytime = pps_to_delay(pps)
  sm.put(delaytime)
  print(delaytime, "us looking for", pps, "Hz")
  sleep(10)

  pps = 200
  delaytime = pps_to_delay(pps)
  sm.put(delaytime)
  print(delaytime, "us looking for", pps, "Hz")
  sleep(10)

  pps = 18
  delaytime = pps_to_delay(pps)
  sm.put(delaytime)
  print(delaytime, "us looking for", pps, "Hz")
  sleep(10)

import select
import sys
import machine
import utime

# # Set u4p the poll object
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
