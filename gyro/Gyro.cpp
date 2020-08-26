#include <iostream>
#include <wiringPiI2C.h>
#include <stdlib.h>
#include <stdio.h>
#include <wiringPi.h>
#include <math.h>
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
  int timing = millis();
  
  cout << "Gyro:Run() in a separate thread : " << timing << std::endl;

  mpu6050.set_defaults();

  mpu6050.calibrate();
  
  cout << "Gyro:Run() DONE in a separate thread : " << (millis() - timing) << std::endl;

  return(0);
}
