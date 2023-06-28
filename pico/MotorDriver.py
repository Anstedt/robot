from rp2 import PIO, StateMachine, asm_pio
from machine import Pin
from time import sleep, sleep_ms, sleep_us, ticks_us

import select
import sys

# See PICO-Poll for comm. with PI
@asm_pio(set_init=PIO.OUT_HIGH)
def pulse_control():
    label("main")
    pull(noblock)
    mov(x,osr)          # save x so a pull without data returns x
    jmp(not_x, "main")  # If x is 0 stop pulsing till we get a non-zero value
    set(pins, 1) [3]    # Turn pin on for 4 1mhz clocks cycles or 4 us delay
    set(pins, 0)        # Turn pin off
    mov(y,osr)          # Now get the low delay time
    label("delay")
    pull(noblock)       # Keep getting the latest value or x if no new values
    mov(x,osr)          # Remember mov() is right to left
    jmp(y_dec, "delay")
    jmp("main")         # Jump back to the beginning

# MotorDriver encapsulates the motor interface. It loads and starts the PIO on
# instantiation.It parses the comm string and controls the appropriate PIO
# interfaces as well as the GPIO direction control, which is not implemented
# yet.
class MotorDriver:
  def __init__(self):
    self.sma = sma = StateMachine(1, pulse_control, freq=1000000, set_base=Pin(16))  # Instantiate SM1, 2000 Hz, LED on pin 3
    self.smb = StateMachine(2, pulse_control, freq=1000000, set_base=Pin(17))  # Instantiate SM1, 2000 Hz, LED on pin 3
    self.sma.active(1)                                                 # Start State Machine 1
    self.smb.active(2)                                                 # Start State Machine 1

  def pps_to_delay(self, val):
    if (val <= 0):
      return(0)
    return(int(((1/val)*1000000)/3) - 4)

  # For now we will assume 1 is forward, 2 is reverse.
  # Remeber we need to handle that each motor is opposite the other
  def get_dir(self, i): # integer
    if (0x8000 & i):
      return(1)
    else:
      return(0)

  def convert_to_int(self, s):
    return(int(s, 16))

  def convert_to_pps(self, i):
    # HJA: We can range check here since a range of 18 to 6400 is plenty
    # All lower bits are speed in pps
    return(0x7fff & i)

  def split_lut(self,luts):
    sma_lut = ""
    smb_lut = ""
    lut_buf = ""

    if (len(luts) < 10):
      print("Error command ", luts, " is less than 10")
      return(sma_lut, smb_lut)

    i = luts.index("x") # Find ms a string start
    sma_lut = luts[i+1:i+5]

    i = luts.index("y") # Find ms a string start
    smb_lut = luts[i+1:i+5]

    return(sma_lut, smb_lut)

  def set_motor_dir(self, m, dir):
    # When implemented we will need to check which motor we are and reverse direction for one versus the other
    # print("Not Implemented dir =", dir)
    return()

  # Parse input string to the 2 motors
  def parse_com(self, s):
    (sma_s, smb_s) = self.split_lut(s)

    sma_i = self.convert_to_int(sma_s)
    self.set_motor_dir(self.sma, self.get_dir(sma_i))

    smb_i = self.convert_to_int(smb_s)
    self.set_motor_dir(self.smb, self.get_dir(smb_i))

    print(sma_s, " sma_i =", sma_i, " pps =", self.convert_to_pps(sma_i))
    print(smb_s, " smb_i =", smb_i, " pps =", self.convert_to_pps(smb_i))

    del_a = self.pps_to_delay(self.convert_to_pps(sma_i))
    del_b = self.pps_to_delay(self.convert_to_pps(smb_i))

    #print(sma_s, "del_a =", del_a, " sma_i =", sma_i)
    if (self.sma.tx_fifo() != 0):
        print("sma buffer =", self.sma.tx_fifo())
    self.sma.put(del_a)

    #print(smb_s, "del_b =", del_b, " smb_i =", smb_i)
    if (self.smb.tx_fifo() != 0):
        print("smb buffer =", self.smb.tx_fifo())
    self.smb.put(del_b)

#
# Test System
#

#
# Test System 1 sends test commands to the MotorDriver and times the sequencing
#
# xFFFF is sma and y is smb, upper but for each motor is direction
emulate_lut = [ "x8120y8120", # 18
                "x8640y8640", # 100 Stop, HJA hack to handle design does not handle 0 speed yet
                "x8C80y8C80", # 200 Speed up
                "x8190y8190", # 400 Slow down
                "x83E8y83E8", # 1000 Got different speeds
                "x87D0y87D0", # 2000
                "x8F00y8F00"] # Stop

def main_1():
  lut_cnt = -1
  ticks4ms = 0
  print("Create motor")
  motor = MotorDriver()
  ticks = ticks_us()

  while True:
    lut_cnt = lut_cnt + 1
    if (lut_cnt >= len(emulate_lut)):
      # print("lut_cnt to big")
      lut_cnt = 0
    # Send current message from lut to motor
    motor.parse_com(emulate_lut[lut_cnt])
    sleep_us(3000) # Tune sleep by experminetation for 250hz
    ticks4ms += 1
    if (ticks4ms >= 250):
      time_diff = (ticks_us() - ticks) / 1000000
      ticks = ticks_us()
      print("ticks4ms = ", ticks4ms, " time seconds =" , time_diff)
      ticks4ms = 0

#
# Test System 2 reads data from the USB port, raspberry pi, and sends it to the MotorDriver
# Thonny cannot be used since it controls the USB connection
#
def main():
  # Create a motor
  motor = MotorDriver()

  # Set up the poll object
  poll_obj = select.poll()
  poll_obj.register(sys.stdin, select.POLLIN)

  print("Start poll loop")
  # Loop indefinitely
  while True:
    # Wait for input on stdin
    poll_results = poll_obj.poll(1) # the '1' is how long it will wait for message before looping again (in microseconds)
    if poll_results:
      # Read the data from stdin (read data coming from PC)
      data = sys.stdin.readline().strip()

      # Send current message from lut to motor
      motor.parse_com(data)

      # Write the data to the input file (print sends data back over USB to the PC)
      # print(data, len(data))
    else:
      # do something if no message received (like feed a watchdog timer)
      continue


if __name__ == '__main__':
  main()

# Test class
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
