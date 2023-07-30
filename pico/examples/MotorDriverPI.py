#!/usr/bin/python3
import serial
import time

# See examples/speedcode.cpp for analysis

def main():
  # s = serial.Serial(port="COM3", parity=serial.PARITY_EVEN, stopbits=serial.STOPBITS_ONE, timeout=1)
  s = serial.Serial('/dev/ttyACM0', parity=serial.PARITY_EVEN, stopbits=serial.STOPBITS_ONE, timeout=1)
  s.flush()

  # Save the time
  save_time = time.time()

  # We have no characters yet
  save_char_cnt = 0

  print("Start Run, Total run should take about 2 second")

  # mystr = "Hello " + "\n"
  for x in range (0,1000):
    mystr = "x00FFy0F00" + "\n"
    s.write(mystr.encode())
    mes = s.read_until()
    save_char_cnt += len(mes)
    print(mes.decode())
    time.sleep(5)
    total_time = time.time() - save_time

  print("chars =", save_char_cnt, "timeS =", save_time, "timeE =", time.time(), "timeT =", total_time)
  print("chars per second =", save_char_cnt / total_time)

if __name__ == "__main__":
  main()
