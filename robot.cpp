#include <iostream>
#include "mpu6050/mpu6050.h"

int main(int argc, char *argv[])
{
  std::cout << "Hello Howie" << std::endl;

  // Run the mpu6050 and capture the return
  return(mpu_run());
}
