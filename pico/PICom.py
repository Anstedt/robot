import serial

def main():
  # s = serial.Serial(port="COM3", parity=serial.PARITY_EVEN, stopbits=serial.STOPBITS_ONE, timeout=1)
  s = serial.Serial('/dev/ttyACM0', parity=serial.PARITY_EVEN, stopbits=serial.STOPBITS_ONE, timeout=1)
  s.flush()

  for x in range (0,1000):
    mystr = "Hello " + str(x) + "\n"
    s.write(mystr.encode())
    mes = s.read_until()
    print(mes.decode())

if __name__ == "__main__":
  main()
