# PICO as motor controller

# Tests in progress
Steps that work
Install MicroPytho, once is enough
Get Thonny talking to PICO
Load up PICO code you want running on PICO
  Test first if you want using green button with arrow
Then in Thonny click File->"save as"
Select PICO
Then save, PICOCom.py for example, as main.py on PICO
Shutdown Thonny
Power cycle PICO, do not hold down button
Run PI program, PICom.py for example

# PI to PICO communications
PI sends value as a hex string, "0x00120xf312", for example which is
commands for both motors.

"0xf312" = motor 1 upper bit is direction 1 and speed "0x7312"
"0x0012" = motor 2 upper bit is direction 0 and speed "0x0012"

# What is Speed to PICO
- could be speed in pulses
- could be delay as part of delay time

_---______-
  ^   ^
  |   |
  |   `- delay from PI
  `- Constant required maximum pulse 1.9 us or higher, used 4us in the past
  
 ^        ^ This is the total pulse time or period and the pulse rate is the
            inverse of this.

# Sending the delay from PI to PICO
PROS:
- Simpler for PICO to handle
- PI has more control of rate information
- PI more easily tunes rate information if PICO timing is a little off
CONS:
- PI needs to know pulse delay, 4us, for example to calculate delay

# Sending direction to PICO
- There are timing specifications on how long before a pulse change
  should there be a delay change
- Step time to actual change in direction is 650ns and should be held
  for another 650ns after that.In our case command will only be sent
  to PICO at 250Hz so worst case is a pulse may occur one time before
  direction changes.
  
