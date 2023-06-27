from rp2 import PIO, StateMachine, asm_pio
from machine import Pin
from time import sleep

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

  def get_dir(self, i): # integer
    # Upper bit is direction
    return(0x8000 & i)

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

# Test class
print("Create motor")
motor = MotorDriver()
motor.sma.put(200)
motor.smb.put(200)

sleep(10)
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
