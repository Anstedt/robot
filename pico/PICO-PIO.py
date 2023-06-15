from machine import Pin
from rp2 import PIO, StateMachine, asm_pio
from time import sleep

# Select 1 bit to use from OUT stream, the in stream for PIO
# Select shift direction
# A default wrap occurs for this case so no loop is needed
# This simply reads the input from the OUT fifo and sends that bit to the LED
@asm_pio(out_init=(rp2.PIO.OUT_HIGH,) * 1, out_shiftdir=PIO.SHIFT_RIGHT, autopull=True)
def led_control():
  pull()
  out(pins, 1)

# GPIO PIN 25 should be the onboard LED for the PICO non-W
sm = StateMachine(0, led_control, freq=10000000, out_base=Pin(25))
sm.active(1)

while True:
  sm.put(1)
  sleep(0.2)
  sm.put(0)
  sleep(0.2)


