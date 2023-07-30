#!/usr/bin/python3
import serial
import time

def main():
  # s = serial.Serial(port="COM3", parity=serial.PARITY_EVEN, stopbits=serial.STOPBITS_ONE, timeout=1)
  s = serial.Serial('/dev/ttyACM0', parity=serial.PARITY_EVEN, stopbits=serial.STOPBITS_ONE, timeout=1)
  s.flush()

  # Save the time
  save_time = time.time()

  # We have no characters yet
  save_char_cnt = 0

  print("Start Run, Total run should take about 2 second")
  
  for x in range (0,1000):
    mystr = "Hello " + str(x) + "\n"
    s.write(mystr.encode())
    #s.write(x)
    #s.write("\n".encode())
    #s.flush()
    # buffer = [0,0,0,0,0]
    # buffer = s.read(5)
    mes = s.read_until()
    # print("Message" , mes.decode(), " len", len(mes))
    save_char_cnt += len(mes)
    # print(buffer, len(buffer))

    total_time = time.time() - save_time
    
  print("chars =", save_char_cnt, "timeS =", save_time, "timeE =", time.time(), "timeT =", total_time)
  print("chars per second =", save_char_cnt / total_time)
  
if __name__ == "__main__":
  main()
