import select
import sys
import time

# Set up the poll object
poll_obj = select.poll()
poll_obj.register(sys.stdin, select.POLLIN)

# Loop indefinitely
while True:
  # Wait for input on stdin
  poll_results = poll_obj.poll(1) # the '1' is how long it will wait for message before looping again (in microseconds)
  if poll_results:
    # Read the data from stdin (read data coming from PC)
    # buffer = sys.stdin.read(4)
    data = sys.stdin.readline().strip()
    # Write the data to the input file (print sends data back over USB to the PC)
    print("received data: ", data, len(data))
    # print(data)
    # sys.stdin.write(buffer)
    #print(len(buffer))
    #sys.stdin.flush()
  else:
    # do something if no message received (like feed a watchdog timer)
    continue
