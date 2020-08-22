#include <iostream>
#include "mpu6050/mpu6050.h"
#include "gyro/Gyro.h"

int main(int argc, char *argv[])
{
  std::cout << "Hello Howie" << std::endl;

  Gyro gyro;

  gyro.Activate();
  
  // Run the mpu6050 and capture the return
  return(mpu_run());
}
