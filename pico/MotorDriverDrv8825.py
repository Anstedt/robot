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

sma = StateMachine(1, pulse_control, freq=1000000, set_base=Pin(16))  # Instantiate SM1, 2000 Hz, LED on pin 3
smb = StateMachine(2, pulse_control, freq=1000000, set_base=Pin(17))  # Instantiate SM1, 2000 Hz, LED on pin 3
sma.active(1)                                                 # Start State Machine 1
smb.active(2)                                                 # Start State Machine 1

def pps_to_delay(val):
  return(int(((1/val)*1000000)/3) - 4)

while True:
  pps = 18
  delaytime = pps_to_delay(pps)
  print(delaytime, "us cycle time, looking for", pps, "Hz")
  sma.put(delaytime)  
  smb.put(delaytime)  
  print(delaytime, "us cycle time, looking for", pps, "Hz")
  sleep(10)

  pps = 6400
  delaytime = pps_to_delay(pps)
  sma.put(delaytime)
  smb.put(delaytime)  
  print(delaytime, "us cycle time, looking for", pps, "Hz")
  sleep(10)
  
  pps = 1600
  delaytime = pps_to_delay(pps)
  sma.put(delaytime)
  smb.put(delaytime)  
  print(delaytime, "us cycle time, looking for", pps, "Hz")
  sleep(10)
  
  pps = 800
  delaytime = pps_to_delay(pps)
  sma.put(delaytime)
  smb.put(delaytime)  
  print(delaytime, "us cycle time, looking for", pps, "Hz")
  sleep(10)

  pps = 400
  delaytime = pps_to_delay(pps)
  sma.put(delaytime)
  smb.put(delaytime)  
  print(delaytime, "us cycle time, looking for", pps, "Hz")
  sleep(10)

  pps = 200
  delaytime = pps_to_delay(pps)
  sma.put(delaytime)
  smb.put(delaytime)  
  print(delaytime, "us cycle time, looking for", pps, "Hz")
  sleep(10)


while True:
  emulate(sma, smb)

import select
import sys
import machine
import utime

# aFFFF is sma and b is smb, rename machines later
emulate_lut = [ "aFFFFbFFFF",
                "a8000b8000", # Stop
                "aFFF0bFFF0", # Speed up
                "a00FFb00FF", # Slow down
                "aFFFFbF00F", # Got different speeds
                "a7FFFb7FFF"] # Change direction

def split_lut(luts):
  {
    sma_lut = ""
    smb_lut = ""
    lut_buf = ""

    if (len(luts) < 10):
    return(sma_lut, smb_lut)

    a = luts.index("a") # Find ms a string start
    sma_lut = a[i+1:i+4]

    a = luts.index("b") # Find ms a string start
    smb_lut = a[i+1:i+4]

    return(sma_lut, smb_lut)
}

def get_dir(i): # integer
  {
    # Upper bit is direction
    return(0x8000 & i)
  }

def convert_to_int(s):
  {
    return(int(s, 16))
  }

def convert_to_pps(i):
  {
    # HJA: We can range check here since a range of 18 to 6400 is plenty
    # All lower bits are speed in pps
    return(0x7fff & i)
  }

lut_cnt = 0
def emulate(msa, msb):
  {
    if (++lut_cnt > len(emulate_lut)):
    lut_cnt = 0

    (sma_s, smb_s) = split_lut(emulate_lut[lut_cnt])

    sma_i = convert_to_int(sma_s)
    set_motor_a_dir(get_dir(sma_i))

    smb_i = convert_to_int(smb_s)
    set_motor_b_dir(get_dir(smb_i))

    del_a = pps_to_delay(convert_to_pps(sma_i))
    del_b = pps_to_delay(convert_to_pps(smb_i))

    sma.put(del_a)
    smb.put(del_b)

    sleep(5)
  }

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
