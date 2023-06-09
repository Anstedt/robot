import select
import sys
import machine
import utime
 
sensor_temp = machine.ADC(4)
adc0 = machine.ADC(0)
adc1 = machine.ADC(1)
adc2 = machine.ADC(2)
adc3 = machine.ADC(3)
conversion_factor = 3.3 / (65535)

while True:
  if select.select([sys.stdin],[],[],0)[0]:
    ch = sys.stdin.readline()
    if ch[0]== "0":
      print("adc ch: " + str(ch[0]) + " : " + str(adc0.read_u16()))
    if ch[0]== "1":
      print("adc ch: " + str(ch[0]) + " : " + str(adc1.read_u16()))
    if ch[0]== "2":
      print("adc ch: " + str(ch[0]) + " : " + str(adc2.read_u16()))
    if ch[0]== "3":
      print("adc ch: " + str(ch[0]) + " : " + str(adc3.read_u16()))
    if ch[0]== "4":
      reading = sensor_temp.read_u16() * conversion_factor
      temperature = 27 - (reading - 0.706)/0.001721
      print("Temp: " + str(temperature))
    else:
      print ("NO data")
      utime.sleep(2)
