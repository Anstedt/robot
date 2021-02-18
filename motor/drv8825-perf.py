#!/usr/bin/python3
from time import sleep
import RPi.GPIO as GPIO

# Right Motor/Motor1
# STEP = 10  # Step GPIO Pin
# DIR  = 18  # Direction GPIO Pin

# Left Motor/Motor2
STEP = 24  # Step GPIO Pin
DIR  = 23  # Direction GPIO Pin

# Both Motors share common mode pins for micro steps
MODE = (17, 27, 22)   # Microstep Resolution GPIO Pins

CW = 1     # Clockwise Rotation
CCW = 0    # Counterclockwise Rotation
SPR = 200  # Steps per Revolution (360 / 7.5)

GPIO.setmode(GPIO.BCM)
GPIO.setup(DIR, GPIO.OUT)
GPIO.setup(STEP, GPIO.OUT)
GPIO.output(DIR, CW)

MODE = (17, 27, 22)   # Microstep Resolution GPIO Pins
GPIO.setup(MODE, GPIO.OUT)
RESOLUTION = {'Full': (0, 0, 0),
              'Half': (1, 0, 0),
              '1/4': (0, 1, 0),
              '1/8': (1, 1, 0),
              '1/16': (0, 0, 1),
              '1/32': (1, 0, 1)}

# GPIO.output(MODE, RESOLUTION['1/32'])
# GPIO.output(MODE, RESOLUTION['Half'])

step_count = SPR * 32
delay = .0208/512
# step_count = SPR
# delay = .0208

for m in range(3, 6):
  if (m == 0):
    GPIO.output(MODE, RESOLUTION['Full'])
    print("Full")
  elif (m == 1):
    GPIO.output(MODE, RESOLUTION['Half'])
    print("Half")
  elif (m == 2):
    GPIO.output(MODE, RESOLUTION['1/4'])
    print("1/4")
  elif (m == 3):
    GPIO.output(MODE, RESOLUTION['1/8'])
    print("1/8")
  elif (m == 4):
    GPIO.output(MODE, RESOLUTION['1/16'])
    print("1/16")
  elif (m == 5):
    GPIO.output(MODE, RESOLUTION['1/32'])
    print("1/32")

  GPIO.output(DIR, CW)
  print("  Clockwise Delay ms:", delay)
  for x in range(step_count):
    GPIO.output(STEP, GPIO.HIGH)
    sleep(2/1000000)
    GPIO.output(STEP, GPIO.LOW)
    sleep(delay)

  sleep(.5)

  GPIO.output(DIR, CCW)
  print("  Counter Clockwise")
  for x in range(step_count):
    GPIO.output(STEP, GPIO.HIGH)
    sleep(2/1000000)
    GPIO.output(STEP, GPIO.LOW)
    sleep(delay)

  sleep(.5)

GPIO.cleanup()
