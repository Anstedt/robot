#include <iostream>
#include <pigpio.h>
#include "gyro/Gyro.h"
#include <unistd.h>

int main(int argc, char *argv[])
{
  std::cout << "Hello Robbie" << std::endl;

  Gyro* p_gyro = new Gyro();

  p_gyro->Activate();

  // Wait for the thread to finish
  p_gyro->JoinThread();

  delete p_gyro;
  
  return(0);
}
