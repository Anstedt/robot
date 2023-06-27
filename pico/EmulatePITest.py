from EmulatePI import EmulatePI
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

emul_pi = EmulatePI()

def main():
  lut_cnt = -1
  lut_len = emul_pi.lut_len() # Does not change while running
  
  while True:
    lut_cnt = lut_cnt + 1
    if (lut_cnt >= lut_len):
      print("lut_cnt to big")
      lut_cnt = 0
    print(lut_cnt, len(emulate_lut))
    emul_pi.emulate(sma, smb, lut_cnt)

    sleep(5)
    
if __name__ == '__main__':
  main()
