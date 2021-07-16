/*******************************************************************************
PACKAGE:  Robot
FILE:     MotorDriver.cpp

PURPOSE:  Controls one motor of the 2 the robot has
*******************************************************************************/
#include <pigpio.h>
#include "Config.h" // HJA should remove this when we have mode setup correctly
#include "MotorDriver.h"

#include "Slog.h"

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
FUNCTION:  MotorDriver::MotorDriver()

ARGUMENTS: steps_rev = number of steps for 1 full revolution, mode = 0
           pulse_gpio = gpio number for pulse control
           dir_gpio = gpio number for direction control
           mode_gpio = array of 3 gpio pins used for mode
           mode = default stepper/chop mode for the motor
           revs_per_min = revolutions per minute
------------------------------------------------------------------------------*/          
MotorDriver::MotorDriver(GPIO pulse_gpio, GPIO dir_gpio, GPIO microstep0, GPIO microstep1, GPIO microstep2)
{
  SLOG << "MotorDriver::MotorDriver()" << std::endl;

  m_motor_pulse_gpio = pulse_gpio;
  m_motor_dir_gpio   = dir_gpio;

  // Copy the gpio mode pin array
  m_motor_mode_gpio[0] = microstep0;
  m_motor_mode_gpio[1] = microstep1;
  m_motor_mode_gpio[2] = microstep2;
  
  m_motor_mode = 0; // Default to 0, MotorCmd will fill this in
  
  // high is always this number of us. Can go as low as 1.9us
  m_pulse_high_us = PULSE_LOW_TIME_US;
  m_pulse_low_us = 0; // Is variable based on requested speed

  if (gpioInitialise() < 0)
  {
    SLOG << "MotorDriver pigpio initialization failed" << std::endl;
  }

  // Initialize gpio motor
  gpioSetMode(m_motor_pulse_gpio, PI_OUTPUT);   // pulse pin
  gpioSetMode(m_motor_dir_gpio, PI_OUTPUT);     // direction pin

  // Mode gpio pins
  gpioSetMode(m_motor_mode_gpio[0], PI_OUTPUT); // mode pin 0
  gpioSetMode(m_motor_mode_gpio[1], PI_OUTPUT); // mode pin 1
  gpioSetMode(m_motor_mode_gpio[2], PI_OUTPUT); // mode pin 2

  // Now set the motor mode
  SetMotorMode(MOTORS_MODE_DEFAULT);

  // Default the direction, note motor 1/2 are apposite direction to go the same
  // way for the robot
  gpioWrite(m_motor_dir_gpio, MOTOR_CW);
}

MotorDriver::~MotorDriver()
{
  // gpioTerminate(); // Close pigpio

  SLOG << "MotorDriver::~MotorDriver()" <<std::endl;
}

/*------------------------------------------------------------------------------
FUNCTION:      MotorDriver::SetMotorMode(int mode)
------------------------------------------------------------------------------*/
bool MotorDriver::SetMotorMode(int mode)
{
  bool status = false;

  // Mode must be 5 or less
  if (mode < 0 || mode > 5)
  {
    SLOG << "MOTOR:mode must be 5 or less, passed = " << mode << "defaulting mode to 0" << std::endl;
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
FUNCTION:  bool MotorCmd(s32 distance_raw, u32 max_speed_raw, u8 microstep_mode)
PURPOSE:   Driver motor at the specified rate at the specified speed

ASSUMES:   steps per revolution is 200

ARGUMENTS: distance_raw  = pulses
                           real distance depends on microstep_mode
                           steps for motor to rotate 360 degrees, normally 200 
                           circumference of wheel
                             often only concerned with angle of rotation
           max_speed_raw = max pulses per second
                           real speed depends on microstep_mode
                           steps for motor to rotate 360 degrees, normally 200 
                           circumference of wheel
                             often only concerned with angle of rotation

EXAMPLE:

steps = steps to take if chopper mode is 1/32 then steps=64 then
motor steps = 2 or 1/100 of a revolution

RETURNS:   worked
------------------------------------------------------------------------------*/
bool MotorDriver::MotorCmd(s32 distance_raw, u32 max_speed_raw, u8 microstep_mode)
{
  bool status = true;

  MotorCMD motor_cmd;
  
  // If not at least =/-4 just set to 0 for driver
  if (distance_raw > -4 && distance_raw < 4)
  {
    distance_raw = 0;
  }
  
  // We will shift mode to meed the required speed and distance
  motor_cmd.steps = distance_raw;
  motor_cmd.speed = max_speed_raw;
  motor_cmd.mode = microstep_mode;
  
  return(m_motor_cmd_fifo.push_back(motor_cmd));
}

/*------------------------------------------------------------------------------
FUNCTION:      MotorDriver::GetPulseLowTime(int pulse_up)
PURPOSE:       Calculate the pulse down time for:
               - Pulse Up Time
               - m_motor_revs_per_min
               - m_motor_steps_rev
               - mode multiplier = motor_mode[m_motor_mode].multiplier;

               - speed is pulses per second


ARGUMENTS:     None
RETURNS:       time in usec to delay
------------------------------------------------------------------------------*/
int MotorDriver::GetPulseLowTime(int pulse_high_us, u32 speed)
{
  int usecs_per_pulse = 1000000/speed; // Convert to microseconds per rev

  // Since we know how long the total pulse should be the low is just the total
  // minus the high pulse
  if ((usecs_per_pulse - pulse_high_us) < 0)
    return(m_pulse_high_us); // Never go negative
  else
    return(usecs_per_pulse - pulse_high_us);
}

/*------------------------------------------------------------------------------
FUNCTION:  MotorDriver::Run(void)
PURPOSE:   Run the motor is a separate thread

Step Frequency                Max: 250kHz
Step Pulse duration STEP High Min: 1.9μs, no max
Step Pulse duration STEP Low  Min: 1.9μs, no max

    1.9us 1.9us
   v     v     v
   ,-----.     ,-----.
   |     |     |     |
---'     `-----'     `--

HJA
HJA MISSING SPEED CONTROL
HJA MISSING MODE CONTROL
HJA
------------------------------------------------------------------------------*/
int MotorDriver::Run(void)
{
  int motor_angle_cmd = 0;

  MotorCMD motor_cmd; // steps, speed in pulses per second, mode

  // Set defaults on startup
  motor_cmd.steps = 0;
  motor_cmd.mode = MOTORS_MODE_DEFAULT;
  
  SLOG << "MotorDriver:Run() in a separate thread" << std::endl;

  uint32_t loop_time_hja = gpioTick();

  while (ThreadRunning())
  {
    // If we have new data, update motor control variables
    if (!m_motor_cmd_fifo.empty())
    {
      motor_cmd = m_motor_cmd_fifo.front();
      m_motor_cmd_fifo.pop_front();

      if (motor_cmd.steps < 0)
      {
        m_motor_dir = MOTOR_CCW;
        motor_cmd.steps *= -1;
      }
      else
      {
        m_motor_dir = MOTOR_CW;
      }

      // Set the direction based on the requested angle
      gpioWrite(m_motor_dir_gpio, m_motor_dir);

      // Set the mode as passed in
      SetMotorMode(motor_cmd.mode);
      
      // SLOG << " Fifo Angle=" << motor_angle_cmd << " Direction=" << m_motor_dir << " steps_to_go=" << m_motor_steps_to_go << std::endl;
    }

    // Run the motor while we have more steps
    if(motor_cmd.steps > 0)
    {
      motor_cmd.steps--;
      // Pulse high time is always the same
      gpioWrite(m_motor_pulse_gpio, 1);

      // Delay Time High: Pulse the motor high then use the actual pulse time to
      // determine the pulse low time.
      m_pulse_low_us = GetPulseLowTime(gpioDelay(m_pulse_high_us), motor_cmd.speed);

      // Delay Time Low: Now do the low pulse
      gpioWrite(m_motor_pulse_gpio, 0);
      gpioDelay(m_pulse_low_us);

      // SLOG << "### steps_to_go=" << m_motor_steps_to_go << std::endl;
      // SLOG << "pulse_gpio=" << m_motor_pulse_gpio << " motor_dir=" << m_motor_dir << " steps_to_go=" << m_motor_steps_to_go << " pulse_low_us=" << m_pulse_low_us << " pulse_high_us="  << m_pulse_high_us << " LoopTime_us=" << ( gpioTick() - loop_time_hja) << std::endl;
    }
    else
    {
      // If we have no data, sleep some waiting on new data, using 2ms which is
      // half gyro rate.
      gpioDelay(500);
    }

    loop_time_hja = gpioTick();
  }

  SLOG << "MotorDriver::Run return" << std::endl;

  return(ThreadReturn());
}
