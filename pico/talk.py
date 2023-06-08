import serial

TERMINATOR = '\r'.encode('UTF8')

ser = serial.Serial('/dev/ttyACM0', 115200, timeout=1)

line='%s\r\f' % 'Howard'

ser.write(line.encode('utf-8'))

inline = ser.read_until(TERMINATOR)

inline.decode('UTF8').strip()

ser.close()
