from machine import Pin
from rp2 import PIO, StateMachine, asm_pio
from time import sleep

@asm_pio(out_init=(rp2.PIO.OUT_HIGH,) * 1, out_shiftdir=PIO.SHIFT_RIGHT, autopull=True, pull_thresh=16)
def paral_prog():
  pull()
  out(pins, 1)

# GPIO PIN 25 should be the onboard LED for the PICO non-W
sm = StateMachine(0, paral_prog, freq=10000000, out_base=Pin(25))
sm.active(1)

while True:
  sm.put(1)
  sleep(0.2)
  sm.put(0)
  sleep(0.2)
