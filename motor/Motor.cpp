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
} motor_mode[7] = {
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
Motor::Motor(int steps_rev, int pulse_gpio, int dir_gpio, const MotorModeGPIO& mode_gpio, int mode, int revs_per_min)
{
  cout << "Motor::Motor()" << std::endl;

  m_motor_steps_rev = steps_rev;
  m_motor_pulse_gpio = pulse_gpio;
  m_motor_dir_gpio = dir_gpio;

  // Copy the gpio mode pin array
  std::copy(std::begin(mode_gpio), std::end(mode_gpio), std::begin(m_motor_mode_gpio));

  m_motor_revs_per_min = revs_per_min;

  m_motor_mode = mode;
  m_motor_steps_to_go = 0;
  // high is always this number of us. Can go as low as 1.9us
  m_pulse_high_us = PULSE_LOW_TIME_US;
  m_pulse_low_us = 0; // Is variable based on requested speed

  if (gpioInitialise() < 0)
  {
    cout << "Motor pigpio initialization failed" << std::endl;
  }

  // Initialize gpio
  gpioSetMode(m_motor_pulse_gpio, PI_OUTPUT);   // pulse pin
  gpioSetMode(m_motor_dir_gpio, PI_OUTPUT);     // direction pin

  // Mode gpio pins
  gpioSetMode(m_motor_mode_gpio[0], PI_OUTPUT); // mode pin 0
  gpioSetMode(m_motor_mode_gpio[1], PI_OUTPUT); // mode pin 1
  gpioSetMode(m_motor_mode_gpio[2], PI_OUTPUT); // mode pin 2

  // Now set the motor mode
  SetMotorMode(mode);

  // Default the direction to CW
  gpioWrite(m_motor_dir_gpio, MOTOR_CW);
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
bool Motor::AddGyroData(int pitch, int yaw, float angle_gyro, float angle_acc)
{
  // cout << "Angle Gyro=" << angle_gyro << "\tAngle Accel=" << angle_acc << "\tGyro Pitch=" << pitch << "\tGyro Yaw=" << yaw << std::endl;

  return(m_angle_gyro_fifo.push_back(angle_gyro));
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
  int motor_angle_cmd = 0;

  cout << "Motor:Run() in a separate thread" << std::endl;

  while (ThreadRunning())
  {
    // If we have new data, update motor control variables
    if (!m_angle_gyro_fifo.empty())
    {
      motor_angle_cmd = m_angle_gyro_fifo.front();
      m_angle_gyro_fifo.pop_front();

      if (motor_angle_cmd < 0)
      {
        m_motor_dir = MOTOR_CW;
        motor_angle_cmd *= -1;
      }
      else
      {
        m_motor_dir = MOTOR_CCW;
      }
      // Convert the angle to steps based on the current chopper mode
      m_motor_steps_to_go = AngleToSteps(motor_angle_cmd);

      // Set the direction based on the requested angle
      gpioWrite(m_motor_dir_gpio, m_motor_dir);

      // cout << " Fifo Angle=" << motor_angle_cmd << " Direction=" << m_motor_dir << " steps_to_go=" << m_motor_steps_to_go << std::endl;
    }

    // Run the motor while we have more steps
    if(m_motor_steps_to_go > 0)
    {
      m_motor_steps_to_go--;
      // Pulse high time is always the same
      gpioWrite(m_motor_pulse_gpio, 1);

      // Delay Time High: Pulse the motor high then use the actual pulse time to
      // determine the pulse low time.
      m_pulse_low_us = GetPulseLowTime(gpioDelay(m_pulse_high_us));

      // Delay Time Low: Now do the low pulse
      gpioWrite(m_motor_pulse_gpio, 0);
      gpioDelay(m_pulse_low_us);
      // cout << " steps_to_go=" << m_motor_steps_to_go << " pulse_low_us=" << m_pulse_low_us << " pulse_high_us="  << m_pulse_high_us << std::endl;
    }
    else
    {
      // If we have no data, sleep some waiting on new data, using 2ms which is
      // half gyro rate.
      gpioDelay(500);
    }
  }

  cout << "Motor::Run return" << std::endl;

  return(ThreadReturn());
}
