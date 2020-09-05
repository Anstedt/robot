#include <iostream>
#include <pigpio.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "Gyro.h"

#include <unistd.h>

using namespace std;

Gyro::Gyro()
{
  m_callback = 0;
  m_start = 0;
  m_gyro_pitch_data_raw = 0;
  m_gyro_yaw_data_raw = 0;
  m_angle_acc = 0.0;
  m_angle_gyro = 0.0;

  if (gpioInitialise() < 0)
  {
    cout << "Gyro pigpio initialization failed" << std::endl;
  }
  else
  {
    p_mpu6050 = new MPU6050();
  }
  
  cout << "Gyro::Gyro()" <<std::endl;
}

Gyro::~Gyro()
{
  delete p_mpu6050;
  
  gpioTerminate(); // Now that the MPU6050 is gone we can close pigpio

  cout << "Gyro::~Gyro()" <<std::endl;
};

/*------------------------------------------------------------------------------
FUNCTION:      bool Gyro::RegisterForCallback(std::function<void(int)> callback)
------------------------------------------------------------------------------*/
bool Gyro::RegisterForCallback(std::function<void(int, int, float, float)> callback)
{
  if (m_callback == 0)
  {
    m_callback = callback;
  }
  
  return(true);
}

#define GYRO_LOOPS 10000

int Gyro::Run(void)
{
  uint32_t timer = 0;
  uint32_t elapsed = 0;
  
  cout << "Gyro:Run() in a separate thread" << std::endl;

  p_mpu6050->set_defaults();

  p_mpu6050->calibrate();

  // Loops should take 4 ms
  timer = gpioTick() + 4000; // gpioTick is in micro seconds
  elapsed = gpioTick();

  for (int i = 0; i < GYRO_LOOPS; i++)
  {
    m_angle_acc = asin((float)(p_mpu6050->get_accel_Z_cal())/8200.0)* 57.296; //Calculate the current angle according to the accelerometer

    // On startup use accelerometer since that is the best we have
    if(m_start == 0)
    {
      m_angle_gyro = m_angle_acc; //Load the accelerometer angle in the angle_gyro variable
      m_start = 1; //Set the start variable to start the PID controller
    }

    // p_mpu6050->get_gyro_XY(m_gyro_yaw_data_raw, m_gyro_pitch_data_raw);
    m_gyro_yaw_data_raw = p_mpu6050->get_gyro_X_cal();
    m_gyro_pitch_data_raw = p_mpu6050->get_gyro_Y_cal();

    m_angle_gyro += m_gyro_pitch_data_raw * 0.000031; //Calculate the traveled during this loop angle and add this to the angle_gyro variable

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //MPU-6050 offset compensation
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Not every gyro is mounted 100% level with the axis of the robot. This can be cause by misalignments during manufacturing of the breakout board.
    //As a result the robot will not rotate at the exact same spot and start to make larger and larger circles.
    //To compensate for this behavior a VERY SMALL angle compensation is needed when the robot is rotating.
    //Try 0.0000003 or -0.0000003 first to see if there is any improvement.

    //Uncomment the following line to make the compensation active
    m_angle_gyro -= m_gyro_yaw_data_raw * 0.0000003;                  //Compensate the gyro offset when the robot is rotating
    m_angle_gyro = m_angle_gyro * 0.9996 + m_angle_acc * 0.0004;      //Correct the drift of the gyro angle with the accelerometer angle

    if (m_callback)
    {
      m_callback(m_gyro_yaw_data_raw, m_gyro_pitch_data_raw, m_angle_gyro, m_angle_acc);
    }
    
    // CallBack now has all data
    // cout << "Angle Gyro=" << m_angle_gyro << "\tAngle Accelerometer=" << m_angle_acc << "\tGyro Yaw=" << m_gyro_yaw_data_raw << "\tGyro Pitch=" << m_gyro_pitch_data_raw << std::endl;

    while(timer > gpioTick());
    timer += 4000;
  }

  cout << "Gyro:Run() DONE in a separate thread : " << (gpioTick() - elapsed)/GYRO_LOOPS << "us" << std::endl;

  return(0);
}
