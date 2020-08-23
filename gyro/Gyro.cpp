#include <iostream>
#include <wiringPiI2C.h>
#include <stdlib.h>
#include <stdio.h>
#include <wiringPi.h>
#include <math.h>
#include "../mpu6050/mpu6050.h"
#include "Gyro.h"

#include <unistd.h>

using namespace std;

Gyro::Gyro()
{
  cout << "Gyro::Gyro()" <<std::endl;
}

Gyro::~Gyro()
{
  cout << "Gyro::~Gyro()" <<std::endl;  
};

int Gyro::Run(void)
{
  cout << "Gyro:Run() in a separate thread" << std::endl;

  while(1)
  {
    cout << "Gyro:Run()" << std::endl;
    usleep(10000);
  }
  return(0);
}
