/*******************************************************************************
PACKAGE: Robot
FILE:    MotorsDriver.cpp

PURPOSE: Low level driver access to motor 
*******************************************************************************/

/* NOTES ***********************************************************************
gcc -o MotorsDriver MotorsDriver.cpp
*******************************************************************************/

/* INCLUDE ********************************************************************/
#include <stdio.h>
#include <string.h>

#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()
#include <cstdio>

/* CLASSES ********************************************************************/
/* GLOBALS ********************************************************************/
/* FUNCTIONS ******************************************************************/
int main()
{
  unsigned char msg[] = { 'M', 'e', 'l', 'l', 'o', '\r' };
  char read_buf [256];
  int serial_port = open("/dev/ttyACM0", O_RDWR);

  if (serial_port < 0)
  {
    printf("Error %i from open: %s\n", errno, strerror(errno));
  }
  else
  {
    printf("Serial Port opened with FH %d\n", serial_port);

    write(serial_port, msg, sizeof(msg));
    fsync(serial_port);

    sleep(1); // Hack to give PICO time to send, real method uses termios to
              // control timing
    
    memset(&read_buf, '\0', sizeof(read_buf));

    // Read bytes. The behaviour of read() (e.g. does it block?,
    // how long does it block for?) depends on the configuration
    // settings above, specifically VMIN and VTIME
    int num_bytes = read(serial_port, &read_buf, sizeof(read_buf));

    // n is the number of bytes read. n may be 0 if no bytes were received, and can also be -1 to signal an error.
    if (num_bytes < 0)
    {
      printf("Error reading: %s", strerror(errno));
    }
    else
    {
    
      // Here we assume we received ASCII data, but you might be sending raw bytes (in that case, don't try and
      // print it to the screen like this!)
      printf("Read %i bytes. Received message: %s\n", num_bytes, read_buf);
    }
  }
  
  close(serial_port);
  
  // printf("Hello\n");
  return(0);
}

/* METHODS ********************************************************************/
