from machine import Pin

# use onboard LED which is controlled by Pin 25
# on a Pico W the onboad lLED is accessed differently,
# so commeent out the line below
# and uncomment the line below that
led = Pin(25, Pin.OUT) # veresion for Pico
# led = Pin('LED', Pin.OUT) # version for Pico W


# Turn the LED on
def on():
    led.value(1)

# Turn the LED off
def off():
    led.value(0)
