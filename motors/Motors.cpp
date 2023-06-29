/*******************************************************************************
PACKAGE: Robot
FILE:    Motors.cpp

PURPOSE: Interface to PICO for motor control 
*******************************************************************************/

/* NOTES ***********************************************************************
g++ Motors.cpp -o Motors -lpigpio
sudo ./Motors
*******************************************************************************/

/* INCLUDE ********************************************************************/
#include "Motors.h"
#include <cstdio>
#include<unistd.h>
#include <cstring>
#include <pigpio.h>

/* CLASSES ********************************************************************/
/* FUNCTIONS ******************************************************************/
Motors::Motors()
{
  char tty[] = "/dev/ttyACM0";
  
  printf("Hello World\n");

  
  if (gpioInitialise() < 0)
  {
    printf("gpio init failed\n");
  }

  int serial = serOpen(tty, 115200, 0);

  if (serial >= 0)
  {
    printf("serial=%s\n", serial);

    char mystr[] = "x00FFy0F00\n";
    int wr = serWrite(serial, mystr, 11);
    if (wr != 0)
      printf("wr=%d\n", wr);

    sleep(1);
    
    char buf[100];
    memset(&buf, '\0', sizeof(buf));
    int rr = serRead(serial, buf, sizeof(buf));
    if (rr >=0)
      printf("rr=\%d\n%s", rr, buf);
          
    serClose(serial);
  }
  else
    printf("ERROR: serial=%s", serial);

  gpioTerminate();
}

int main()
{
  Motors* p_motors = new Motors();

  delete p_motors;
}

/* METHODS ********************************************************************/
