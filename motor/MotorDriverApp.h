/*******************************************************************************
PACKAGE:       Robot
FILE:          MotorDriver.h
*******************************************************************************/
#ifndef MOTOR_H
#define MOTOR_H

/* INCLUDE ********************************************************************/
#include "Threads.h"
#include "Fifo.h"

using MotorModeGPIO = int[3];

/*------------------------------------------------------------------------------
CLASS:	  MotorDriver
PURPOSE:  Control a stepper motor
------------------------------------------------------------------------------*/
class MotorDriver : public Threads
{
public:
  // Local Classes
  // Constructors
  // int (&mode_gpio)[3];
  MotorDriver(int steps_rev, int pulse1_gpio, int dir1_gpio, int pulse2_gpio, int dir2_gpio, const MotorModeGPIO& mode_gpio, int motor_mode, int revs_per_min = 1);
  // Mutators: non-const operations
  bool AddGyroData(int pitch, int yaw, float angle_acc, float angle_gyro);
  bool SetMotorMode(int mode);
  // Accessors: const operations
  // Static and friend functions
  // Memory management: copy constructor, destructor, operator=
  ~MotorDriver();
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
  MotorDriver(const MotorDriver&);
  MotorDriver& operator=(const MotorDriver& rhs);
  // Data fields
  int m_motor_steps_rev;
  int m_motor1_pulse_gpio;
  int m_motor1_dir_gpio;
  int m_motor2_pulse_gpio;
  int m_motor2_dir_gpio;
  MotorModeGPIO m_motor_mode_gpio;
  int m_motor_revs_per_min;
  // Motor control
  int m_motor_steps_to_go;
  int m_motor_mode;
  int m_motor1_dir;
  int m_motor2_dir;
  int m_pulse_high_us;
  int m_pulse_low_us;
  Fifo<float, 512> m_angle_gyro_fifo;
  // Static (shared) class variables
};

#endif /* MOTOR_H */
