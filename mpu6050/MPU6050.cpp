#include <iostream>
#include <pigpio.h>
#include <stdlib.h>
#include <stdio.h>
#include <wiringPi.h>
#include <math.h>
#include "MPU6050.h"
#include <unistd.h> // for usleep()

using namespace std;

MPU6050::MPU6050()
{
  // Initializes I2C with device Address
  cout << "MPU6050::MPU6050()" << std::endl;

  if (gpioInitialise() < 0)
  {
    cout << "MPU6050::~MPU6050() pigpio initialization failed" << std::endl;
  }

  // RP uses bus 1
  device_fd = i2cOpen(1, MPU6050::Device_Address, 0);
}

MPU6050::~MPU6050()
{
  i2cClose(device_fd);

  gpioTerminate();

  cout << "MPU6050::~MPU6050()" << std::endl;
}

// Setup  defaults if the user request them
void MPU6050::set_defaults(void)
{
  // See MPU6050_INIT in mpu6050.demo.c for potential sequencing
  set_power_mgmt(0x00);

  // Set our defaults
  set_gyro_config(0x00);   // 250dps full scale
  set_accel_config(0x08);  // (+/- 4g full scale range)
  set_filter_config(0x03); // (Set Digital Low Pass Filter to ~43Hz)
}

// Get and set calibration values
void MPU6050::calibrate(void)
{
  gyro_yaw_calibration_value = 0;
  gyro_pitch_calibration_value = 0;

  int timer = millis() + 4;
  int elapsed = millis();
  
  // Loop 500 times
  for(int counter = 0; counter < 500; counter++)
  {
    // Note the real could pulls both values from I2C in one call
    gyro_yaw_calibration_value   += get_gyro_X();
    gyro_pitch_calibration_value += get_gyro_Y();
    // DEBUG cout << "gyro_yaw_calibration_value  =" << gyro_yaw_calibration_value << std::endl;
    // DEBUG cout << "gyro_pitch_calibration_value=" << gyro_pitch_calibration_value << std::endl;
    //Wait for 3700 microseconds to simulate the main program loop time
    
    while(timer > millis());
    timer += 4;
  }
  gyro_pitch_calibration_value /= 500; // Divide the total value by 500 to get the avarage gyro offset
  gyro_yaw_calibration_value /= 500;            

  cout << "Elapsed time = " << float(((millis() - elapsed) / 500)) << std::endl;
  cout << "gyro_pitch_calibration_value=" << gyro_pitch_calibration_value << std::endl;
  cout << "gyro_yaw_calibration_value  =" << gyro_yaw_calibration_value << std::endl;
}

// Set power management configs5
void MPU6050::set_power_mgmt(int value)
{
  i2cWriteByteData(device_fd, PWR_MGMT_1, value);
}

void MPU6050::set_gyro_config(int value)
{
  i2cWriteByteData(device_fd, GYRO_CONFIG, value);
}

void MPU6050::set_accel_config(int value)
{
  i2cWriteByteData(device_fd, ACCEL_CONFIG, value);
}

void MPU6050::set_filter_config(int value)
{
  i2cWriteByteData(device_fd, CONFIG, value);
}

int MPU6050::get_gyro_X(void)
{
  return(read_raw_data(GYRO_XOUT_H));
}

int MPU6050::get_gyro_Y(void)
{
  return(read_raw_data(GYRO_YOUT_H));
}

int MPU6050::get_gyro_Z(void)
{
  return(read_raw_data(GYRO_ZOUT_H));
}

void MPU6050::get_gyro_XY(int &x, int &y)
{
  // Until we can do this with one command we will just use 2
  x = read_raw_data(MPU6050::GYRO_XOUT_H);
  y = read_raw_data(MPU6050::GYRO_YOUT_H);

  // HJA char buffer[4];
  // HJA gets bot X and Y in one call
  // HJA i2cReadBlockData(device_fd, GYRO_XOUT_H, buffer);
  // HJA x = buffer[0]<<8|buffer[1];
  // HJA y = buffer[2]<<8|buffer[3];
}

int MPU6050::get_accel_X(void)
{
  return(read_raw_data(ACCEL_XOUT_H));
}

int MPU6050::get_accel_Y(void)
{
  return(read_raw_data(ACCEL_YOUT_H));
}

int MPU6050::get_accel_Z(void)
{
  return(read_raw_data(ACCEL_ZOUT_H));
}

long MPU6050::get_gyro_yaw_calibration_value()
{
  return(gyro_yaw_calibration_value);
}

long MPU6050::get_gyro_pitch_calibration_value()
{
  return(gyro_pitch_calibration_value);
}

int MPU6050::read_raw_data(int addr)
{
  short int value;

  value = i2cReadWordData(device_fd, addr);
  value = ((value<<8)&0xff00)|((value>>8)&0x00ff);

	return value;
}
