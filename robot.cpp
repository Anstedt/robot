#include <iostream>
// #include "mpu6050/MPU6050.h"
#include "gyro/Gyro.h"
#include <unistd.h>

int main(int argc, char *argv[])
{
  std::cout << "Hello Howie" << std::endl;

  Gyro gyro;

  gyro.Activate();

  // sleep(10);
  
  // Run the mpu6050 and capture the return

  // Wait for the thread to finish
  gyro.JoinThread();
  
  return(0);
}
