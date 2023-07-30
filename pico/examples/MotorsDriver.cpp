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

#include <ostream>
#include <iostream>
#include <cstring>

#include <memory>
#include <string>
#include <stdexcept>

template<typename ... Args>
std::string string_format( const std::string& format, Args ... args )
{
    int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    auto size = static_cast<size_t>( size_s );
    std::unique_ptr<char[]> buf( new char[ size ] );
    std::snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}

using namespace std;

/* CLASSES ********************************************************************/
/* GLOBALS ********************************************************************/
/* FUNCTIONS ******************************************************************/
int main()
{
  unsigned char msg1[6] = { 'M', 'e', 'l', 'l', 'o', '\n' };

  // Note adding /n to string add a / and n to the string but we need the actual
  // control character instead so we can stick it at the end. Note this is no
  // longer a proper string because it does not have null at the end.
  unsigned char msg2[] = "Hello World";
  unsigned char msg3[] = "Hello World NOW\n";
  msg2[sizeof(msg2)-1] = '\n';
  
  //string s = string_format("{:x}", 42); // s == 2a
  //printf("s=%s\n", s.c_str());
  
  printf("msg1 %s size=%d\n", msg1, sizeof(msg1));
  printf("msg2 %s size=%d\n", msg2, sizeof(msg2));
  printf("msg3 %s size=%d\n", msg3, sizeof(msg3));

  // Hex conversion example
  char s1[100];
  char s2[100];
  char s3[100];
  char s4[100];
  
  int s1s = sprintf(s1, "%#.4X\n", 49999);
  int s2s = sprintf(s2, "%#.4x\n", 49999);
  int s3s = sprintf(s3, "%#.4X\n", 15);
  int s4s = sprintf(s4, "%#.4x\n", 99);
  
  printf("s1 %s", s1);
  printf("s2 %s", s2);
  printf("s3 %s", s3);
  printf("s4 %s", s4);
  
      
  char read_buf [256];
  int serial_port = open("/dev/ttyACM0", O_RDWR);

  struct termios tty;
  
  if (serial_port < 0)
  {
    printf("Error %i from open: %s\n", errno, strerror(errno));
  }
  else
  {
    // Init
    if(tcgetattr(serial_port, &tty) != 0)
    {
      printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
      return 1;
    }
    
    tty.c_cc[VTIME] = 1; // Wait for up to 0.1s (1 deciseconds)
    tty.c_cc[VMIN] = 29; //returning as soon as any data is received.

    // Save tty settings, also checking for error
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0)
    {
      printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
    }

    printf("Serial Port opened with FH %d\n", serial_port);

    dprintf(serial_port, "%s", msg3);
    
    // write(serial_port, msg2, sizeof(msg2));
    fsync(serial_port);

    //sleep(1); // Hack to give PICO time to send, real method uses termios to
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
