/*******************************************************************************
PACKAGE:       Robot
FILE:          Motors.h
*******************************************************************************/
#ifndef MOTORS_H
#define MOTORS_H

/* INCLUDE ********************************************************************/
#include "LockingQueue.h"

#include "PID.h"
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
  int GetPulseLowTime(int pulse_high_us);
  int AngleToSteps(float angle);
  // Static and friend functions
  // Memory management
  Motors(const Motors&);
  Motors& operator=(const Motors& rhs);
  // Data fields
  int m_motor_steps_rev;
  int m_motor_revs_per_min;
  u8 m_motor_mode; // In our case this will be the same for both motors

  // Motor control
  s32 m_motor1_distance; // +/- controls direction, 0 is stop
  u32 m_motor1_speed;    // steps/second
  s32 m_motor1_dir;      // this is 1 or -1 since each motor goes in the opposite direction
  
  s32 m_motor2_distance; // +/- controls direction, 0 is stop
  u32 m_motor2_speed;    // steps/second
  s32 m_motor2_dir;      // this is 1 or -1 since each motor goes in the opposite direction

  MotorsDriver m_motorsDriver;

  LockingQueue<float> m_angle_gyro_fifo;
  // Static (shared) class variables
};

#endif /* MOTORS_H */
