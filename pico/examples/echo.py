# all these imports come from MicroPython. https://docs.micropython.org/en/latest/index.html
import uselect
from machine import Pin, SPI, PWM, RTC
import framebuf
import time
import random
import gc
import math
from sys import stdin, exit
import micropython
import sys

led = Pin(25, Pin.OUT) # HJA

# size of each letter in pixels
CHARACTER_SIZE = 8

# how serial lines are ended
TERMINATOR = "\n"


class Pico:
    """
    Global singleton, so we can use `self`. instead of global.
    Not sure if this will increase ram usage.
    """

    def __init__(self):
        # give a chance to break the boot to fix serial/code issues. Put any riskier startup code after this
        boot_delay_seconds = 5

        self.run_loop = True

        led.value(0) # HJA
        # store incomplete lines from serial here. list of strings (no typing module in micropython)
        self.buffered_input = []
        # when we get a full line store it here, without the terminator.
        # gets overwritten if a new line is read (as early as next tick).
        # blanked each tick.
        self.input_line_this_tick = ""

    def main(self):
        counter = 0

        latest_input_line = ""

        # main loop
        while self.run_loop:

            # buffer from the USB to serial port
            self.read_serial_input()

            ########################### app per tick code here

            # simple output test
            print(counter)
            counter += 1

            # show serial input on the screen.
            # only update if we have a new line
            if self.input_line_this_tick:
                latest_input_line = self.input_line_this_tick

            ########################### end app per tick code here

            # simple loop speed control
            time.sleep_ms(100)

    def read_serial_input(self):
        led.value(0)
        # stdin.read() is blocking which means we hang here if we use it. Instead use select to tell us if there's anything available
        # note: select() is deprecated. Replace with Poll() to follow best practises
        select_result = uselect.select([stdin], [], [], 0)
        print("HJA", select_result)
        while select_result[0]:
            # there's no easy micropython way to get all the bytes.
            # instead get the minimum there could be and keep checking with select and a while loop
            led.value(1)
            input_character = stdin.read(1)
            # add to the buffer
            self.buffered_input.append(input_character)
            # check if there's any input remaining to buffer
            select_result = uselect.select([stdin], [], [], 0)
            print("HJA Got")
        # if a full line has been submitted
        led.value(0)
        if TERMINATOR in self.buffered_input:
            line_ending_index = self.buffered_input.index(TERMINATOR)
            # make it available
            self.input_line_this_tick = "".join(self.buffered_input[:line_ending_index])
            # remove it from the buffer.
            # If there's remaining data, leave that part. This removes the earliest line so should allow multiple lines buffered in a tick to work.
            # however if there are multiple lines each tick, the buffer will continue to grow.
            if line_ending_index < len(self.buffered_input):
                self.buffered_input = self.buffered_input[line_ending_index + 1 :]
            else:
                self.buffered_input = []
        # otherwise clear the last full line so subsequent ticks can infer the same input is new input (not cached)
        else:
            self.input_line_this_tick = ""

    def delay_boot(self, seconds):
        """
        Wait for the given amount of time, allowing breaking with key_a and key_b at the same time,
        to avoid getting stuck with bad code.
        """
        tick_ms_timestamp = time.ticks_ms()
        delta_time_ms = 0
        timer_ms = 0
        max_ms = seconds * 1000

        while timer_ms < max_ms:
            self.read_input()
            if self.key_a_pressed and self.key_b_pressed:
                self.lcd.text("boot cancelled", 1, 32, 0xFFFF)
                self.lcd.show()
                exit()

            delta_time_ms = time.ticks_diff(time.ticks_ms(), tick_ms_timestamp)
            tick_ms_timestamp = time.ticks_ms()
            timer_ms += delta_time_ms

    def exit(self):
        self.run_loop = False

# start the code
if __name__ == "__main__":
    pico = Pico()
    pico.main()
    # when the above exits, clean up
    gc.collect()
