/*******************************************************************************
PACKAGE:  Robot
FILE:     Motor.cpp

PURPOSE:  Controls one motor of the 2 the robot has
*******************************************************************************/
#include <pigpio.h>
#include <unistd.h>
#include "Config.h"
#include "Motor.h"

#include "Slog.h"

using namespace std;

/*------------------------------------------------------------------------------
FUNCTION:  Motor::Motor()

ARGUMENTS: steps_rev = number of steps for 1 full revolution, mode = 0
           pulse_gpio = gpio number for pulse control
           dir_gpio = gpio number for direction control
           mode_gpio = array of 3 gpio pins used for mode
           mode = default stepper/chop mode for the motor
           revs_per_min = revolutions per minute
------------------------------------------------------------------------------*/
Motor::Motor(int steps_rev, GPIO pulse_gpio, GPIO dir_gpio, GPIO microstep0, GPIO microstep1, GPIO microstep2, int mode, int revs_per_min, int dir)
{
  SLOG << "Motor::Motor()" << std::endl;

  m_motorDriver = new MotorDriver(pulse_gpio, dir_gpio, microstep0, microstep1, microstep2);

  m_motorDriver->Activate(SCHED_FIFO, 1); // Make the motor the highest priority
  
  m_motor_steps_rev = steps_rev;

  m_motor_dir = dir;
  m_motor_revs_per_min = revs_per_min;

  // Now set the motor mode
  SetMotorMode(mode);
}

/*------------------------------------------------------------------------------
FUNCTION: Motor:: Motor()
------------------------------------------------------------------------------*/
Motor::~Motor()
{
  SLOG << "Motor::~Motor()" <<std::endl;

  // Remove Right MotorDriver
  SLOG << "Motor MotorDriver->JoinThread" << std::endl;
  m_motorDriver->StopThreadRun();
  while(!m_motorDriver->ThreadStopped())
  {
    SLOG << "Waiting for MotorDriver to stop" << std::endl;
    sleep(1);
  }

  m_motorDriver->JoinThread();

  SLOG << "Motor delete MotorDriver" << std::endl;
  delete m_motorDriver;
}

/*------------------------------------------------------------------------------
FUNCTION:      Motor::AddGyroData(int pitch, int yaw, float angle_gyro, float angle_acc)
RETURNS:       None
------------------------------------------------------------------------------*/
bool Motor::AddGyroData(int y, int x, float angle_gyro, float angle_acc)
{
  // SLOG << "Angle Gyro=" << angle_gyro << "\tAngle Accel=" << angle_acc << "\tGyro Y=" << y << "\tGyro X=" << x << std::endl;
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
FUNCTION:      int Motor::AngleToSteps(float angle)
RETURNS:       None
------------------------------------------------------------------------------*/
int Motor::AngleToSteps(float angle)
{
  // Motor mode lookup
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
FUNCTION:  Motor::Run(void)
PURPOSE:   Run the motor is a separate thread
------------------------------------------------------------------------------*/
int Motor::Run(void)
{
  float motor_angle_cmd = 0;
  int m_motor_steps_to_go = 0;
  
  SLOG << "Motor:Run() in a separate thread" << std::endl;

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
      m_motorDriver->MotorCmd(m_motor_steps_to_go, (m_motor_revs_per_min * 200 * 32) / 60, m_motor_mode);

      // SLOG << " Fifo Angle=" << motor_angle_cmd << " Direction=" << m_motor_dir << " steps_to_go=" << m_motor_steps_to_go << std::endl;
    }
  }

  SLOG << "Motor::Run return" << std::endl;

  return(ThreadReturn());
}
