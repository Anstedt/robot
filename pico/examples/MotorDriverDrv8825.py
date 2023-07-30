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

sma = StateMachine(1, pulse_control, freq=1000000, set_base=Pin(16))  # Instantiate SM1, 2000 Hz, LED on pin 3
smb = StateMachine(2, pulse_control, freq=1000000, set_base=Pin(17))  # Instantiate SM1, 2000 Hz, LED on pin 3
sma.active(1)                                                 # Start State Machine 1
smb.active(2)                                                 # Start State Machine 1

def pps_to_delay(val):
  if (val <= 0):
    return(0)
  return(int(((1/val)*1000000)/3) - 4)

def get_dir(i): # integer
  # Upper bit is direction
  return(0x8000 & i)

def convert_to_int(s):
  return(int(s, 16))

def convert_to_pps(i):
  # HJA: We can range check here since a range of 18 to 6400 is plenty
  # All lower bits are speed in pps
  return(0x7fff & i)

# aFFFF is sma and b is smb, rename machines later
emulate_lut = [ "x8012y8012", # 18
                "x8064y8064", # 100 Stop, HJA hack to handle design does not handle 0 speed yet
                "x80C8y80C8", # 200 Speed up
                "x8190y8190", # 400 Slow down
                "x83E8y83E8", # 1000 Got different speeds
                "x87D0y87D0", # 2000
                "x8000y8000"] # Stop

def split_lut(luts):
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

def emulate(msa, msb, lut_cnt):
  (sma_s, smb_s) = split_lut(emulate_lut[lut_cnt])

  sma_i = convert_to_int(sma_s)
  # set_motor_a_dir(get_dir(sma_i))

  smb_i = convert_to_int(smb_s)
  # set_motor_b_dir(get_dir(smb_i))

  print(lut_cnt, sma_s, " sma_i =", sma_i, " pps =", convert_to_pps(sma_i))
  print(lut_cnt, smb_s, " smb_i =", smb_i, " pps =", convert_to_pps(sma_i))

  del_a = pps_to_delay(convert_to_pps(sma_i))
  del_b = pps_to_delay(convert_to_pps(smb_i))

  print(sma_s, "del_a =", del_a, " sma_i =", sma_i)
  sma.put(del_a)

  print(smb_s, "del_b =", del_b, " smb_i =", smb_i)
  smb.put(del_b)

  sleep(10)

def main():
  lut_cnt = -1

  while True:
    lut_cnt = lut_cnt + 1
    if (lut_cnt >= len(emulate_lut)):
      print("lut_cnt to big")
      lut_cnt = 0
    print(lut_cnt, len(emulate_lut))
    emulate(sma, smb, lut_cnt)

if __name__ == '__main__':
    main()

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
