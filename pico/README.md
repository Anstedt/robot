## PICO as motor controller

# Design Bug
- Current design does not handle speeds of 0.
  - Maybe loop in sm until we get something other than 0

## Requirements
# PPS Range
  - Minimum 1 degree per second or a delay of 0.05625 seconds
    - 32x200/360 = 17.7... PPS
    - 1/17.7... = 0.05625 seconds
	- This does effect the design since speed data comes at 250hz or
      0.004 seconds. Handling Speed Range below.
  - Maximum 1600 PPS which is a delay of 1/1600 = 0.000625
    - 32x200 = 360 degrees of rotation
	- 1/4 turn = 32x200/4 = 1600 pulses per second to get 1/4 turn per second
	- NOTE: Even though this is a delay of 6.4 pulses per 250 hz. This
      does not effect the design since high rates are faster than the
      250Hz calls.
  - Maximum down time 620, Better to just use down X 3 + up 
    - 1600 PPS = 1/1600 seconds / pulse = 625us
	- delay time = 625/3 - 4
    - Delay time for 1600 pps is up+down=625us. Down should be 625-4=621-1=620
    - 204 delay is 16.03 khz

    - Delay loop is 3 cycles for each delay unit. So 204 delay in loop
      is really 612. Really 615 since it loops till after zero then
      loops back to mainloop which is another 4 for a total of 616,
      not quiet 625 but at least close enough. Really a little close
      than that since the total time includes the set() [] time which
      is another 4. So we are at 620 total pulse time.
	
- Minimum positive pulse is 1.7 us. Previously used 4us with no
  issues. We may need to go slower so we can get the total PPS range
  we need.

# Handling Speed Range
- Always use the latest data in the buffer or use the x value if no
  new data.

- Start state machines right after each other to minimize time
  shift. Not really important in this application but could be in
  other more time sensitive applications.

- y_dec takes one loop when y is one. Makes sense after to think about it.
  - y_dec indicates y-- decrements after the jmp check

- Total on time is controlled by line, set(pins, 1) [3]
  - 4us for 1000000 PIO clock
  
- Total off time is dealy loops plus full path back to on time line.
  - See pps_to_delay() on handling this
	
- NOTE: mov(x/y, osr) does not clear osr

- The mov(x,osr) call preserves x so a pull on an empty buffer returns
  the same value over and over, needed for fast delays. This works
  because pull with no data uses the current x value instead

# Design PICO
- The PICO C or Python side of the PICO system
- Read data from PI
- Strip off direction bit
- MAYBE adjust based on known PIO timing
- Send data to PIO, one for each motor
- Handle different rates for each PIO-Motor
  - This is caused by different pulse delays sent to each motor for turning.
  - Initially this is not needed since each motor runs the same speed.
- Prevent overflow of PIO FIFO's.

# Design PIO
- The PIO assemble, Python or assembler created side of the PICO system
- PIO reads data as fast as it can, read on every pulse
  - If no new data uses old data again
    pull(noblock) # If empty osr is copied from x
	# mov is copy so src is not modified as part of transfer
    mov(x,osr)    # Copy osr to X to prepare for future noblock pulls
    mov(y,osr)    # This way we either use newest data or last data
    # we need this for high speed pulses that should occur faster than
	# the 250Hz rate data is sent to the PIO, 250Hz.

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

If PICO set for 1,000,000 then I need 4 delays per pulse which is
4us. Which is greater than the required 1.9us.


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
  
