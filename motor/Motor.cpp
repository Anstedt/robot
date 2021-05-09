/*******************************************************************************
PACKAGE:  Robot
FILE:     Motor.cpp

PURPOSE:  Controls one motor of the 2 the robot has
*******************************************************************************/
#include <iostream>
#include <pigpio.h>
#include "Motor.h"

using namespace std;

#define MOTOR_CW  1
#define MOTOR_CCW 0
#define PULSE_LOW_TIME_US 4

struct MotorMode
{
  int pin_0;
  int pin_1;
  int pin_2;
  int multiplier;
} motor_mode[6] = {
  {0,0,0, 1},   // Full
  {1,0,0, 2},   // Half
  {0,1,0, 4},   // 1/4
  {1,1,0, 8},   // 1/8
  {0,0,1, 16},  // 1/16
  {1,0,1, 32}}; // 1/32

/*------------------------------------------------------------------------------
FUNCTION:  Motor::Motor()

ARGUMENTS: steps_rev = number of steps for 1 full revolution, mode = 0
           pulse_gpio = gpio number for pulse control
           dir_gpio = gpio number for direction control
           mode_gpio = array of 3 gpio pins used for mode
           mode = default stepper/chop mode for the motor
           revs_per_min = revolutions per minute
------------------------------------------------------------------------------*/
Motor::Motor(int steps_rev, GPIO pulse_gpio, GPIO dir_gpio, GPIO microstep0, GPIO microstep1, GPIO microstep2, int mode, int revs_per_min)
  : m_motorDriver(pulse_gpio, dir_gpio, microstep0, microstep1, microstep2)
{
  cout << "Motor::Motor()" << std::endl;

  m_motor_steps_rev = steps_rev;

  m_motor_dir_gpio   = dir_gpio;

  m_motor_revs_per_min = revs_per_min;

  // Now set the motor mode
  SetMotorMode(mode);
}


Motor::~Motor()
{
  gpioTerminate(); // Close pigpio

  cout << "Motor::~Motor()" <<std::endl;
}

/*------------------------------------------------------------------------------
FUNCTION:      Motor::AddGyroData(int pitch, int yaw, float angle_gyro, float angle_acc)
RETURNS:       None
------------------------------------------------------------------------------*/
bool Motor::AddGyroData(int y, int x, float angle_gyro, float angle_acc)
{
  // cout << "Angle Gyro=" << angle_gyro << "\tAngle Accel=" << angle_acc << "\tGyro Y=" << y << "\tGyro X=" << x << std::endl;

  m_angle_gyro_fifo.push(angle_gyro);

  return(true);
}

/*------------------------------------------------------------------------------
FUNCTION:      Motor::SetMotorMode(int mode)
------------------------------------------------------------------------------*/
bool Motor::SetMotorMode(int mode)
{
  bool status = false;

  // Mode must be 5 or less
  if (mode < 0 || mode > 5)
  {
    cout << "MOTOR:mode must be 5 or less, passed = " << mode << "defaulting mode to 0" << std::endl;
    m_motor_mode = 0;
  }
  else
  {
    m_motor_mode = mode;
    status = true;
  }

  gpioWrite(m_motor_mode_gpio[0], motor_mode[m_motor_mode].pin_0);
  gpioWrite(m_motor_mode_gpio[1], motor_mode[m_motor_mode].pin_1);
  gpioWrite(m_motor_mode_gpio[2], motor_mode[m_motor_mode].pin_2);

  return(status);
}

/*------------------------------------------------------------------------------
FUNCTION:      Motor::GetPulseLowTime(int pulse_up)
PURPOSE:       Calculate the pulse down time for:
               - Pulse Up Time
               - m_motor_revs_per_min
               - m_motor_steps_rev
               - mode multiplier = motor_mode[m_motor_mode].multiplier;

ARGUMENTS:     None
RETURNS:       time in usec to delay
------------------------------------------------------------------------------*/
int Motor::GetPulseLowTime(int pulse_high_us)
{
  int usecs_per_rev = (60*1000000)/m_motor_revs_per_min; // Convert to microseconds per rev
  int pulses_per_rev =  motor_mode[m_motor_mode].multiplier * m_motor_steps_rev;
  int usecs_per_pulse = usecs_per_rev / pulses_per_rev;

  // Since we know how long the total pulse should be the low is just the total
  // minus the high pulse
  if ((usecs_per_pulse - pulse_high_us) < 0)
    return(m_pulse_high_us); // Never go negative
  else
    return(usecs_per_pulse - pulse_high_us);
}

/*------------------------------------------------------------------------------
FUNCTION:      int Motor::AngleToSteps(float angle)
RETURNS:       None
------------------------------------------------------------------------------*/
int Motor::AngleToSteps(float angle)
{
  int pulses_per_rev =  motor_mode[m_motor_mode].multiplier * m_motor_steps_rev;

  // pulses for 1 revolution are the number of pulses needed for 1 revolution
  // 1 revolution is 360 degrees
  // So if we need to turn say 60 degrees we need (60/360) of the pulses
  return(pulses_per_rev * (angle / 360));
}

/*------------------------------------------------------------------------------
FUNCTION:  Motor::Run(void)
PURPOSE:   Run the motor is a separate thread

Step Frequency                Max: 250kHz
Step Pulse duration STEP High Min: 1.9μs, no max
Step Pulse duration STEP Low  Min: 1.9μs, no max

    1.9us 1.9us
   v     v     v
   ,-----.     ,-----.
   |     |     |     |
---'     `-----'     `--
------------------------------------------------------------------------------*/
int Motor::Run(void)
{
  float motor_angle_cmd = 0;

  cout << "Motor:Run() in a separate thread" << std::endl;

  uint32_t loop_time_hja = gpioTick();

  while (ThreadRunning())
  {
    // Wait up to 100ms for the data then try again on next loop
    // Normally data comes at 4ms but on shutdown that may not happen
    if (m_angle_gyro_fifo.tryWaitAndPop(motor_angle_cmd, 100))
    {
      if (motor_angle_cmd < 0)
      {
        m_motor_dir = MOTOR_CCW;
        motor_angle_cmd *= -1;
      }
      else
      {
        m_motor_dir = MOTOR_CW;
      }

      // Convert the angle to steps based on the current chopper mode
      m_motor_steps_to_go = AngleToSteps(motor_angle_cmd);

      // HJA At this point we can call Ricks driver

      // Set the direction based on the requested angle
      gpioWrite(m_motor_dir_gpio, m_motor_dir);

      // cout << " Fifo Angle=" << motor_angle_cmd << " Direction=" << m_motor_dir << " steps_to_go=" << m_motor_steps_to_go << std::endl;
    }
  }

  cout << "Motor::Run return" << std::endl;

  return(ThreadReturn());
}
