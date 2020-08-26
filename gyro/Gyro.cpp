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

  for(int x=0; x < 1000; x++)
  {
    cout << "Gyro:Run() x=" << x << std::endl;
    usleep(1000);
  }

  cout << "Gyro:Run() DONE in a separate thread : " << (millis() - timing) << std::endl;

  return(0);
}
