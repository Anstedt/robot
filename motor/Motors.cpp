/*******************************************************************************
PACKAGE:  Robot
FILE:     Motors.cpp

PURPOSE:  Controls one motor of the 2 the robot has
*******************************************************************************/
#include "Config.h"
#include "Motors.h"

#include "CmdLine.h"

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
  : m_thread_speed(0), m_thread_speed_cnt(0), m_input_degrees(0), m_output_speed(0), m_setpoint(0),
    m_pid(&m_input_degrees, &m_output_speed, &m_setpoint, (PID_Kp*MOTOR_ANGLE_RATIO), PID_Ki, PID_Kd, DIRECT),
    m_motorsDriver(m1_pulse_gpio, m1_dir_gpio, m2_pulse_gpio, m2_dir_gpio, microstep0, microstep1, microstep2, p_driver_mutex)
{
  SLOG << "Motors::Motors()" << std::endl;

  double kp, ki, kd;
  if (CmdLine::Instance()->GetKp(&kp))
  {
    if (CmdLine::Instance()->GetKi(&ki))
    {
      if (CmdLine::Instance()->GetKd(&kd))
      {
        // All values passed on command line
        SLOG << "PID kp=" << kp << " ki=" << ki << " kd=" << kd << std::endl;
        m_pid.SetTunings(kp*MOTOR_ANGLE_RATIO, ki, kd);
      }
    }
  }
  
  m_pid.SetSampleTime(PRIMARY_THREAD_RATE);
  m_pid.SetOutputLimits(-MOTORS_MAX_PULSES_PER_SEC, MOTORS_MAX_PULSES_PER_SEC);
  m_pid.SetMode(AUTOMATIC);
      
  // Setup for motor 1
  m_motor1_dir = MOTOR1_DIRECTION;     // this is 1 or -1 since each motor goes in the opposite direction
  
  // Setup for motor 2  
  m_motor2_dir = MOTOR2_DIRECTION;     // this is 1 or -1 since each motor goes in the opposite direction
  
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
   
  // HJA SLOG << "Angle Gyro=" << angle_gyro << "\tAngle Accel=" << angle_acc << "\tGyro Y=" << y << "\tGyro X=" << x << std::endl;
  
  // Find the speed we need, then the distance
  speed = AngleToSpeed(angle_gyro, &distance);
  
  if (speed >= PRIMARY_THREAD_RATE) // 250Hz
  {
    // If the driver is controlling the speed, reset the thread speed control
    m_thread_speed = 0;
    m_thread_speed_cnt = 0;
  
    // HJA SLOG << "TEST DRIVER speed=" << speed << " distance=" << distance << " angle_gyro=" << angle_gyro << std::endl;

    // Make calls directly to the driver for rates higher than the thread rate
    DriverRateControl(speed, distance);
  }
  else
  {
    // HJA SLOG << "TEST THREAD speed=" << speed << " distance=" << distance << " angle_gyro=" << angle_gyro << std::endl;

    // For lower rates us the thread to determine when to call the driver
    ThreadRateControl(speed, distance);
  }
  
  return(true);
}

/*------------------------------------------------------------------------------
FUNCTION:  Motors::DriverRateControl(u32 speed, s32 distance)
PURPOSE:       

ARGUMENTS: rate: motor pulses per second
           distance: motor distance in steps

RETURNS:   true: all went well
------------------------------------------------------------------------------*/
bool Motors::DriverRateControl(u32 speed, s32 distance)
{
  bool ret;

  // HJA SLOG << "Motors:DriverRateControl speed=" << speed << " distance=" << distance << std::endl;  

  // Note distance has already been adjusted for thread rate and mode
  // distance still need adjust for rotation direction
  // Tell motors to go the same speed and distance
  ret = m_motorsDriver.MotorsCmdSimple(speed, distance, m_motor_mode);

  return(ret);
}

/*------------------------------------------------------------------------------
FUNCTION:  Motors::ThreadRateControl(s32 speed, u32 distance)
PURPOSE:   Control the speed when the it is less than 250 Pulses Per Second
           We do that by calling the driver at the rate we need and telling it
           speed 250 and distance 2, one over what we need.

ARGUMENTS: rate: motor pulses per second
           distance: motor distance in steps

RETURNS:   true: all went well
------------------------------------------------------------------------------*/
bool Motors::ThreadRateControl(u32 speed, s32 distance)
{
  bool ret;

  // SLOG << "Motors:ThreadRateControl speed=" << speed << " distance=" << distance << std::endl;  

  // If the speed is not zero we need to clock the driver
  if (speed != 0)
  {
    // If the new speed is greater than the old one, go to the new speed. For a
    // new speed lower than the new speed, let the old speed clock the driver
    // until it finishes
    if (speed > m_thread_speed)
    {
      m_thread_speed = speed;

      // Set counter to the count down to the next clock of the driver
      m_thread_speed_cnt = PRIMARY_THREAD_RATE / speed;

      // To prevents missed pulses, clock the driver on a speed increase
      ret = m_motorsDriver.MotorsCmdSimple(speed, distance, m_motor_mode);
    }

    // Clock the driver based on the counter
    if (--m_thread_speed_cnt <= 0)
    {
      ret = m_motorsDriver.MotorsCmdSimple(speed, distance, m_motor_mode);

      // Since we have clocked the driver, get the new speed
      m_thread_speed = speed;
    }
  }
  else
  {
    // If speed is zero clear for next speed command
    m_thread_speed = 0;
    m_thread_speed_cnt = 0;
    ret = true;
  }

  return(ret);
}

/*------------------------------------------------------------------------------
FUNCTION: Motors::AngleToSpeed(float angle);
PURPOSE:  Use angle and mode to determine the speed

          Should have PID as part of this but for now just hack in something
          simple. Basically the bigger the error the greater the speed.  In out
          case error should be 0.

RETURNS:  speed : pluses per second
------------------------------------------------------------------------------*/
u32 Motors::AngleToSpeed(float angle, s32* distance)
{
  bool dist_reverse = false;
  u32 speed = 0;
  
  m_input_degrees = angle; // This has a range of +/-180
  m_setpoint = 0; // HJA really never changes but we make sure it is set here
    
  // After everything is setup in the constructor we are ready to go
  m_pid.Compute();

  // In our system speed is always positive, since distance handles direction
  if (m_output_speed < 0)
  {
    m_output_speed *= -1;

    // If the speed was negative distance needs to be negative
    dist_reverse = true;
  }

  speed = m_output_speed;
  
  // Make sure the driver has enough distance even if we are 2 periods late
  *distance = (speed / PRIMARY_THREAD_RATE) * 2;

  // negate distance if speed was negative, note at this point speed is positive
  if (dist_reverse)
    *distance *= -1;
  
  return(speed);
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

/*
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
*/

