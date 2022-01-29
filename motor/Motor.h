/*******************************************************************************
PACKAGE:       Robot
FILE:          Motor.h
*******************************************************************************/
#ifndef MOTOR_H
#define MOTOR_H

/* INCLUDE ********************************************************************/
#include "Threads.h"
#include "LockingQueue.h"

#include "PID.h"
#include "StandardTypes.h"
#include "rpi4-stepper.h"
#include "MotorDriver.h"

/*------------------------------------------------------------------------------
CLASS:	  Motor
PURPOSE:  Control a stepper motor
------------------------------------------------------------------------------*/
class Motor : public Threads
{
public:
  // Local Classes
  // Constructors
  Motor(int steps_rev, GPIO pulse_gpio, GPIO dir_gpio, GPIO a, GPIO b, GPIO c, int motor_mode, int revs_per_min, int direction, pthread_mutex_t* p_driver_mutex);
  // Mutators: non-const operations
  bool AddGyroData(int pitch, int yaw, float angle_acc, float angle_gyro);
  bool SetMotorMode(int mode);
  // Accessors: const operations
  // Static and friend functions
  // Memory management: copy constructor, destructor, operator=
  ~Motor();
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
  Motor(const Motor&);
  Motor& operator=(const Motor& rhs);
  // Data fields
  int m_motor_steps_rev;
  int m_motor_revs_per_min;
  // Motor control
  int m_motor_mode;
  int m_motor_dir;
  MotorDriver m_motorDriver;

  LockingQueue<float> m_angle_gyro_fifo;
  // Static (shared) class variables
};

#endif /* MOTOR_H */
