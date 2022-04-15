/*******************************************************************************
PACKAGE:  Robot
FILE:     Motors.cpp

PURPOSE:  Controls one motor of the 2 the robot has
*******************************************************************************/
#include <pigpio.h>
#include "Config.h"
#include "Motors.h"

#include "Slog.h"

using namespace std;

/*------------------------------------------------------------------------------
FUNCTION:  Motors::Motors()

ARGUMENTS: motor1/2_pulse_gpio = gpio number for pulse control
           motor1/2_dir_gpio = gpio number for direction control
           mode_gpio = array of 3 gpio pins used for mode
           mutex = locking mutex for driver
------------------------------------------------------------------------------*/
Motors::Motors(GPIO m1_pulse_gpio, GPIO m1_dir_gpio, GPIO m2_pulse_gpio, GPIO m2_dir_gpio, GPIO microstep0, GPIO microstep1, GPIO microstep2, pthread_mutex_t* p_driver_mutex)
  : m_motorsDriver(m1_pulse_gpio, m1_dir_gpio, m2_pulse_gpio, m2_dir_gpio, microstep0, microstep1, microstep2, p_driver_mutex)
{
  SLOG << "Motors::Motors()" << std::endl;

  // Setup for motor 1
  m_motor1_distance = 0;               // +/- controls direction, 0 is stop
  m_motor1_speed = 0;                  // steps/second
  m_motor1_dir = MOTOR1_DIRECTION;     // this is 1 or -1 since each motor goes in the opposite direction
  
  // Setup for motor 2  
  m_motor1_distance = 0;               // +/- controls direction, 0 is stop
  m_motor1_speed = 0;                  // steps/second
  m_motor1_dir = MOTOR2_DIRECTION;     // this is 1 or -1 since each motor goes in the opposite direction
  
  // Now set the motor mode
  SetMotorsMode(MOTORS_MODE_DEFAULT); // In most cases this will be the same for both motors
}

/*------------------------------------------------------------------------------
FUNCTION: Motors:: Motors()
------------------------------------------------------------------------------*/
Motors::~Motors()
{
  SLOG << "Motors::~Motors()" <<std::endl;
}

/*------------------------------------------------------------------------------
FUNCTION:  Motors::AddGyroData(int pitch, int yaw, float angle_gyro, float angle_acc)
PURPOSE:   Use the Gyro data to control the motors

RETURNS:   None
------------------------------------------------------------------------------*/
bool Motors::AddGyroData(int y, int x, float angle_gyro, float angle_acc)
{
  u32 speed; // The pulses per second for the motors, using angle AND mode
  s32 distance; // Distance based on speed, mode, as well as 250Hz thread rate
   
  // SLOG << "Angle Gyro=" << angle_gyro << "\tAngle Accel=" << angle_acc << "\tGyro Y=" << y << "\tGyro X=" << x << std::endl;

  // HJA At this point we will call the driver cmd may take x calls, such as
  // HJA Init motor 1 array
  // HJA Init motor 2 array
  // HJA Send array
  // HJA MotorsDriver::MotorsCmd(
  // m1_distance, m2_distance
  // m1_speed, m2_speed
  // m1_mode, m2_mode, maybe we will delete this
  // m_angle_gyro_fifo.push(angle_gyro);

  // April 08, 2022
  // At this point we need to convert angle_gyro to distance and we need speed
  // from thee PID. In reality we need speed and rate worked out so Rick can
  // finish in on thread run. So we only need rate.
  // Example rate = 500 then distance is

  // Find the rate we need, then the distance
  speed = AngleToRate(angle_gyro);
  distance = RateToDistance(speed);
  
  if (speed > PRIMARY_THREAD_PERIOD) // 250Hz
    // Make calls directly to the driver for rates higher than the thread rate
    DriverRateControl(speed, distance);
  else
    // For lower rates us the thread to determine when to call the driver
    ThreadRateControl(speed, distance);
  
  return(true);
}

/*------------------------------------------------------------------------------
FUNCTION:    Motors::bool DriverRateControl(int rate, int distance)
PURPOSE:       

ARGUMENTS:   rate: motor pulses per second
             distance: motor distance in steps

RETURNS:     true: all went well
------------------------------------------------------------------------------*/
Motors::bool DriverRateControl(int rate, int distance)
{
  m_motorDriver.MotorCmd(rate, distance, m_motor_mode);
}

/*------------------------------------------------------------------------------
FUNCTION:    Motors::bool ThreadRateControl(int rate, int distance)
PURPOSE:       

ARGUMENTS:   rate: motor pulses per second
             distance: motor distance in steps

RETURNS:     true: all went well
------------------------------------------------------------------------------*/
Motors::bool ThreadRateControl(int rate, int distance)
{
}

/*------------------------------------------------------------------------------
FUNCTION:  Motors::AngleToSpeed(float angle);
PURPOSE:   Use angle and mode to determine the speed

RETURNS:   speed : pluses per second
------------------------------------------------------------------------------*/
u32 Motors::AngleToSpeed(float angle, u8 mode);
{
}

/*------------------------------------------------------------------------------
FUNCTION:  s32 Motors::SpeedToDistance(u32 speed);
PURPOSE:   Use speed, mode and thread rate to determine distance

RETURNS:       None
------------------------------------------------------------------------------*/
s32 Motors::SpeedToDistance(u32 speed);
{
}

/*------------------------------------------------------------------------------
FUNCTION:      Motors::SetMotorsMode(int mode)
------------------------------------------------------------------------------*/
bool Motors::SetMotorsMode(int mode)
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

  return(status);
}

/*------------------------------------------------------------------------------
FUNCTION:      int Motors::AngleToSteps(float angle)
RETURNS:       None
------------------------------------------------------------------------------*/
int Motors::AngleToSteps(float angle)
{
  // Motors mode lookup
  const int motor_mode[6] = {
    1,   // Full
    2,   // Half
    4,   // 1/4
    8,   // 1/8
    16,  // 1/16
    32}; // 1/32

  int pulses_per_rev =  motor_mode[m_motor_mode] * m_motor_steps_rev;

  // pulses for 1 revolution are the number of pulses needed for 1 revolution
  // 1 revolution is 360 degrees
  // So if we need to turn say 60 degrees we need (60/360) of the pulses
  return(pulses_per_rev * (angle / 360));
}

/*------------------------------------------------------------------------------
FUNCTION:  Motors::Run(void)
PURPOSE:   Run the motor is a separate thread
------------------------------------------------------------------------------*/
/*
int Motors::Run(void)
{
  float motor_angle_cmd = 0;
  int m_motor_steps_to_go = 0;
  
  SLOG << "Motors:Run() in a separate thread" << std::endl;

  uint32_t loop_time_hja = gpioTick();

  while (ThreadRunning())
  {
    // Wait up to 100ms for the data then try again on next loop
    // Normally data comes at 4ms but on shutdown that may not happen
    if (m_angle_gyro_fifo.tryWaitAndPop(motor_angle_cmd, 100))
    {
      // Convert the angle to steps based on the current chopper mode
      m_motor_steps_to_go = AngleToSteps(motor_angle_cmd);

      // Correct motor direction since each motor runs in the opposite direction
      m_motor_steps_to_go *= m_motor_dir;
      
      // HJA At this point we can call Ricks driver
      // HJA Issue here is the mode should be in the equation rather than 32 constant
      // MOTORS_RPM_DEFAULT = 30 rpm
      // / 60 = rp second = 0.5
      // * 200 which is pulses per rev of the motor = 100
      // * mode modifier for example 1/32 = 100 * 32 = 3200
      m_motorDriver.MotorCmd(m_motor_steps_to_go, (m_motor_revs_per_min * 200 * 32) / 60, m_motor_mode);

      // SLOG << " Fifo Angle=" << motor_angle_cmd << " Direction=" << m_motor_dir << " steps_to_go=" << m_motor_steps_to_go << std::endl;
    }
  }

  SLOG << "Motors::Run return" << std::endl;

  return(ThreadReturn());
}
*/
