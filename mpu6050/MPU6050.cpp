/*******************************************************************************
PACKAGE:  Robot
FILE:     MPU6050.cpp

PURPOSE:  Interface to the MPU6050 Gyro/Accel via I2C
*******************************************************************************/
#include <pigpio.h>
#include <math.h>
#include "MPU6050.h"

#include "Slog.h"

using namespace std;

/*------------------------------------------------------------------------------
FUNCTION:      MPU6050::MPU6050()
------------------------------------------------------------------------------*/
MPU6050::MPU6050()
{
  // Initializes I2C with device Address
  SLOG << "MPU6050::MPU6050()" << std::endl;

  // We start un-calibrated
  m_calibrated = false;

  // Acceleration data cal is just a constant based on sitting the robot
  // straight up and capturing the raw Z axis data
  m_acc_calibration_value = 513; // 1000;

  // Should not need this but stops i2cClose from complaining on shutdown
  if (gpioInitialise() < 0)
  {
    SLOG << "~MPU6050 pigpio initialization failed" << std::endl;
  }

  // RP uses bus 1
  device_fd = i2cOpen(1, MPU6050::Device_Address, 0);
}

/*------------------------------------------------------------------------------
FUNCTION:      MPU6050::~MPU6050()
------------------------------------------------------------------------------*/
MPU6050::~MPU6050()
{
  i2cClose(device_fd);

  SLOG << "~MPU6050 NOT RUNNING gpioTerminate" << std::endl;
  // gpioTerminate(); // Now that the MPU6050 is gone we can close pigpio

  SLOG << "MPU6050::~MPU6050()" << std::endl;
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
  int accel_raw_avg = 0;
  int accel_raw_min =  10000;
  int accel_raw_max = -10000;

  int accel_raw;
  
  m_gyro_yaw_calibration_value = 0;
  m_gyro_pitch_calibration_value = 0;

  uint32_t timer = gpioTick() + (uint32_t)4000;
  uint32_t elapsed = gpioTick();

  // Loop 500 times
  for(int counter = 0; counter < 500; counter++)
  {
    // Note the real code pulls both values from I2C in one call
    m_gyro_yaw_calibration_value   += get_gyro_X();
    m_gyro_pitch_calibration_value += get_gyro_Y();

    accel_raw = get_accel_Z();
    accel_raw_avg += accel_raw;

    if (accel_raw > accel_raw_max) accel_raw_max = accel_raw;
    if (accel_raw < accel_raw_min) accel_raw_min = accel_raw;
    
    // DEBUG SLOG << "gyro_yaw_calibration_value  =" << gyro_yaw_calibration_value << std::endl;
    // DEBUG SLOG << "gyro_pitch_calibration_value=" << gyro_pitch_calibration_value << std::endl;
    // Wait for 4000 microseconds to simulate the main program loop time

    while(timer > gpioTick());
    timer += (uint32_t)4000;
  }
  m_gyro_pitch_calibration_value /= 500; // Divide the total value by 500 to get the avarage gyro offset
  m_gyro_yaw_calibration_value /= 500;

  accel_raw_avg /= 500;

  // Manual Z accelerometer calibration
  // When you need to manually calibrate Z turn on this log, and set
  // m_acc_calibration_value to -accel_raw_avg.
  // SLOG << "accel_raw_avg=" << accel_raw_avg << std::endl;

  // Now adjust using manual calibration value
  accel_raw_avg += m_acc_calibration_value;
  
  // Calculate the angle according to the accelerometer average
  if (accel_raw_avg > 8200) accel_raw_avg = 8200;
  if (accel_raw_avg < -8200) accel_raw_avg = -8200;

  // Now calculate the Z value
  // The 57.296 is the conversions from radians to degrees. The - is so that
  // leaning forward is positive. The 8200(8192) is from the data sheet for AFS_SEL 1.
  m_acc_Z_cal_ang = (asin((float)(accel_raw_avg) / 8200.0) * -57.296);
  
  SLOG << "Elapsed time = " << float(((gpioTick() - elapsed) / 500)) << "us" << std::endl;
  SLOG << "gyro_pitch_calibration_value=" << m_gyro_pitch_calibration_value << std::endl;
  SLOG << "gyro_yaw_calibration_value  =" << m_gyro_yaw_calibration_value << std::endl;
  SLOG << "m_acc_Z_cal_ang=" << m_acc_Z_cal_ang << " accel_raw=" << accel_raw << " accel_raw_avg with manual calibration=" << accel_raw_avg << std::endl;
  SLOG << "accel_raw_min=" << accel_raw_min << " accel_raw_max=" << accel_raw_max << std::endl;
  
  m_calibrated = true; // We are now calibrated
}

// Set power management configs5
void MPU6050::set_power_mgmt(int value)
{
  int status = i2cWriteByteData(device_fd, PWR_MGMT_1, value);
  
  if (status < 0)
  {
    SLOG << "ERROR set_power_mgmt ret=" << status << std::endl;
  }  
}

void MPU6050::set_gyro_config(int value)
{
  int status = i2cWriteByteData(device_fd, GYRO_CONFIG, value);

  if (status < 0)
  {
    SLOG << "ERROR set_gyro_config ret=" << status << std::endl;
  }
}

void MPU6050::set_accel_config(int value)
{
  int status = i2cWriteByteData(device_fd, ACCEL_CONFIG, value);

  if (status < 0)
  {
    SLOG << "ERROR set_accel_config ret=" << status << std::endl;
  }
}

void MPU6050::set_filter_config(int value)
{
  int status = i2cWriteByteData(device_fd, CONFIG, value);

  if (status < 0)
  {
    SLOG << "ERROR set_filter_config ret=" << status << std::endl;
  }
}

/*------------------------------------------------------------------------------
FUNCTION:      MPU6050::get_gyro_?_cal()
------------------------------------------------------------------------------*/
int MPU6050::get_gyro_X_cal(void)
{
  if (!m_calibrated) calibrate();

  return(read_raw_data(GYRO_XOUT_H) - m_gyro_yaw_calibration_value);
}

int MPU6050::get_gyro_Y_cal(void)
{
  if (!m_calibrated) calibrate();

  return(read_raw_data(GYRO_YOUT_H) - m_gyro_pitch_calibration_value);
}

int MPU6050::get_gyro_Z_cal(void)
{
  if (!m_calibrated) calibrate();

  return(read_raw_data(GYRO_ZOUT_H));
}

void MPU6050::get_gyro_XY_cal(int &x, int &y)
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

/*------------------------------------------------------------------------------
FUNCTION:      MPU6050::get_accel_?_cal()

PURPOSE: Assumes all accel axis have the same calibration constraint
------------------------------------------------------------------------------*/
int MPU6050::get_accel_X_cal(void)
{
  return(get_accel_cal(ACCEL_XOUT_H));
}

int MPU6050::get_accel_Y_cal(void)
{
  return(get_accel_cal(ACCEL_YOUT_H));
}

int MPU6050::get_accel_Z_cal(void)
{
  return(get_accel_cal(ACCEL_ZOUT_H));
}

/*------------------------------------------------------------------------------
FUNCTION: int MPU6050::get_accel_cal(int addr)

PURPOSE: This function assumes all accel axis use the same constant
         offset. Probably not true but ONLY Z access is used for now. The Z
         access constant m_acc_calibration_value was created by setting the
         robot level then checking the raw accel Z value.
------------------------------------------------------------------------------*/
int MPU6050::get_accel_cal(int addr)
{
  int accelerometer_data_cal;

  // Add in the cal value
  accelerometer_data_cal = read_raw_data(addr) + m_acc_calibration_value;

  // Keep values in range
  if (accelerometer_data_cal > 8200) accelerometer_data_cal = 8200;
  if (accelerometer_data_cal < -8200) accelerometer_data_cal = -8200;

  return(accelerometer_data_cal);
}

/*------------------------------------------------------------------------------
FUNCTION: MPU6050::get_gyro_?()

PURPOSE: These are the un-calibrated gyro data for internal use
------------------------------------------------------------------------------*/
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

/*------------------------------------------------------------------------------
FUNCTION: MPU6050::get_accel_?()

PURPOSE: These are the un-calibrated accel data for internal use
------------------------------------------------------------------------------*/
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

/*------------------------------------------------------------------------------
FUNCTION:  MPU6050::read_raw_data(int addr)

ARGUMENTS: addr: mpu6050 address to read

RETURNS:   mpu6050 data for the specified address
------------------------------------------------------------------------------*/
int MPU6050::read_raw_data(int addr)
{
  short int value;

  value = i2cReadWordData(device_fd, addr);
  value = ((value<<8)&0xff00)|((value>>8)&0x00ff);

	return value;
}
