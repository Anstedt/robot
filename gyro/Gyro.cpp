/*******************************************************************************
PACKAGE: Robot
FILE:    Gyro.cpp

PURPOSE: Gyroscope Accelorometer Interface
*******************************************************************************/
#include <iostream>
#include <pigpio.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "Gyro.h"

#include <unistd.h>

using namespace std;

/*------------------------------------------------------------------------------
FUNCTION: Gyro::Gyro()
------------------------------------------------------------------------------*/
Gyro::Gyro()
{
  m_callback = 0;
  m_start = 0;
  m_gyro_Y_data_raw = 0;
  m_gyro_X_data_raw = 0;
  m_angle_acc = 0.0;
  m_angle_gyro = 0.0;

  if (gpioInitialise() < 0)
  {
    cout << "Gyro pigpio initialization failed" << std::endl;
  }
}

/*------------------------------------------------------------------------------
FUNCTION: Gyro::~Gyro()
------------------------------------------------------------------------------*/
Gyro::~Gyro()
{
  cout << "Gyro::~Gyro()" <<std::endl;
};

/*------------------------------------------------------------------------------
FUNCTION:      bool Gyro::RegisterForCallback(std::function<void(int)> callback)

Use the C++ functional interface to register a callback with 4 parameters
------------------------------------------------------------------------------*/
bool Gyro::RegisterForCallback(std::function<void(int, int, float, float)> callback)
{
  // We only allow one callback but a vector of callbacks could be used if needed
  if (m_callback == 0)
  {
    m_callback = callback;
  }

  return(true);
}

/*------------------------------------------------------------------------------
FUNCTION: int Gyro::Run(void)

PURPOSE: Process data for Gyro/Accel and send it registered module
------------------------------------------------------------------------------*/
int Gyro::Run(void)
{
  uint32_t timer = 0;
  uint32_t elapsed = 0;

  int test_stepper = 0;
  int test_stepper_mode = 0;

  int test_angle_inc_dec = 1;
  
  cout << "Gyro:Run() in a separate thread" << std::endl;

  // Each loop should take 4 ms
  timer = gpioTick() + 4000; // gpioTick is in micro seconds
  elapsed = gpioTick();

  while (ThreadRunning())
  {
    if (++test_stepper > 250)
    {
      test_stepper = 0;

      switch (++test_stepper_mode)
      {
        case 1:
          m_angle_gyro = 2;
          break;
        case 2:
          m_angle_gyro = 4;
          break;
        case 3:
          m_angle_gyro = 0;
          break;
        case 4:
          m_angle_gyro = -2;
          break;
        case 5:
          m_angle_gyro = -4;
          break;
        default:
          test_stepper_mode = 0;
      }
    }
    
    if (m_callback)
    {
      m_callback(0, 0, m_angle_gyro, 0);
    }

    // CallBack now has all data
    // cout << "Angle Gyro=" << m_angle_gyro << "\tAngle Accelerometer=" << m_angle_acc << "\tGyro X=" << m_gyro_X_data_raw << "\tGyro Y=" << m_gyro_Y_data_raw << std::endl;
    // cout << "Angle Gyro=" << m_angle_gyro << std::endl;

    while(timer > gpioTick());
    timer += 4000;
  }

  cout << "Gyro:Run() DONE in a separate thread : " << (gpioTick() - elapsed) << "us" << std::endl;

  return(ThreadReturn());
}
