/*******************************************************************************
PACKAGE:       Robot
FILE:          MotorDriver.h
*******************************************************************************/
#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

/* INCLUDE ********************************************************************/
#include "StandardTypes.h"
#include "rpi4-stepper.h"
#include "Threads.h"
#include "Fifo.h"

using MotorModeGPIO = int[3];

typedef struct 
{
  s32 steps;
	u32 speed;
	u8 mode;
} MotorCMD;

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
  int GetPulseLowTime(int pulse_high_us, u32 speed);
  // Static and friend functions
  // Memory management
  MotorDriver(const MotorDriver&);
  MotorDriver& operator=(const MotorDriver& rhs);
  // Data fields
  int m_motor_pulse_gpio;
  int m_motor_dir_gpio;
  MotorModeGPIO m_motor_mode_gpio;

  int m_distance_raw;
  int m_max_speed_raw;
  // int m_motor_microstep_mode;

  // Motor control
  int m_motor_mode;
  int m_motor_dir;
  int m_pulse_high_us;
  int m_pulse_low_us;
  Fifo<MotorCMD, 512> m_motor_cmd_fifo;
  // Static (shared) class variables
};

#endif /* MOTOR_H */
