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
  MotorDriver(GPIO pulse_gpio, GPIO dir_gpio, GPIO microstep0, GPIO microstep1, GPIO microstep2);
  // Mutators: non-const operations
  bool MotorCmd(s32 distance_raw, u32 max_speed_raw, u8 microstep_mode);
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
  int m_motor_pulse_gpio;
  int m_motor_dir_gpio;
  MotorModeGPIO m_motor_mode_gpio;

  struct STEPPER_SETUP m_motor_control;
  int m_distance_raw;
  int m_max_speed_raw;
  int m_motor_microstep_mode;

  // Motor control
  int m_motor_steps_to_go;
  int m_motor_mode;
  int m_motor_dir;
  int m_pulse_high_us;
  int m_pulse_low_us;
  Fifo<float, 512> m_angle_gyro_fifo;
  // Static (shared) class variables
};

#endif /* MOTOR_H */
