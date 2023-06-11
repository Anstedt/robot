#!/usr/bin/env python3

num=64123
hex_str = hex(num)
print("num =", num, " hex_str =", hex_str)
res = int(hex_str,16)
print("hex_str =",hex_str, " res =",res)
print(hex_str.replace("0x", ""))

#Consider sending HEX to PICO with 0x removed for shorter strings

