/*******************************************************************************
PACKAGE:       Robot
FILE:          Motors.h
*******************************************************************************/
#ifndef MOTORS_H
#define MOTORS_H

/* INCLUDE ********************************************************************/
#include "PID_v1.h"
#include "StandardTypes.h"
#include "rpi4-stepper.h"
#include "MotorsDriver.h"

/*------------------------------------------------------------------------------
CLASS:	  Motors
PURPOSE:  Control a stepper motor
------------------------------------------------------------------------------*/
class Motors
{
public:
  // Local Classes
  // Constructors
  Motors(GPIO m1_pulse_gpio, GPIO m1_dir_gpio, GPIO m2_pulse_gpio, GPIO m2_dir_gpio, GPIO mode0, GPIO mode1, GPIO mode2, pthread_mutex_t* p_driver_mutex);
  // Mutators: non-const operations
  bool AddGyroData(int pitch, int yaw, float angle_acc, float angle_gyro);
  bool SetMotorsMode(int mode);
  bool Move(s32 direction, u32 speed);
  bool Turn(int degrees); // Cause robot to rotate, +/- degrees specifies  direction of rotation
  // Accessors: const operations
  // Static and friend functions
  // Memory management: copy constructor, destructor, operator=
  ~Motors();
protected:
  int Run(void); // Thread entry point
private:
  // Local Classes
  // Constructors
  // Mutators: non-const operations
  // Accessors: const operations
  bool DriverRateControl(u32 rate, s32 distance);
  bool ThreadRateControl(u32 rate, s32 distance);
  u32 AngleToSpeed(float angle);
  s32 SpeedToDistance(u32 speed, float angle);
  
  // Static and friend functions
  // Memory management
  Motors(const Motors&);
  Motors& operator=(const Motors& rhs);
  // Data fields

  // Motor control
  u8 m_motor_mode; // In our case this will be the same for both motors
  s32 m_motor1_dir;      // this is 1 or -1 since each motor goes in the opposite direction
  s32 m_motor2_dir;      // this is 1 or -1 since each motor goes in the opposite direction

  // Thread rate control
  int m_thread_speed;
  int m_thread_speed_cnt;
  
  MotorsDriver m_motorsDriver;

  // Static (shared) class variables
};

#endif /* MOTORS_H */
