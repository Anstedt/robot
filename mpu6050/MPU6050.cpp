#include <wiringPiI2C.h>
#include <stdlib.h>
#include <stdio.h>
#include <wiringPi.h>
#include <math.h>
#include "MPU6050.h"

using namespace std;

MPU6050::MPU6050()
{
  /*Initializes I2C with device Address*/
  device_fd = wiringPiI2CSetup(MPU6050::Device_Address);
}

MPU6050::~MPU6050()
{
}

void MPU6050::set_gyro_config(int register, in value)
{
}

short MPU6060::read_raw_data(int addr)
{
  short high_byte,low_byte,value;
  value = wiringPiI2CReadReg16(device_fd, addr);
  value = ((value<<8)&0xff00)|((value>>8)&0x00ff);
	return value;
}
