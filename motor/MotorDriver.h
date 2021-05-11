/*******************************************************************************
PACKAGE: Robot
FILE:    MotorDriver.h

PURPOSE: Low level motor control interface
*******************************************************************************/

/* NOTES ***********************************************************************
*******************************************************************************/

#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

/* INCLUDE ********************************************************************/
#include "StandardTypes.h"
#include "rpi4-stepper.h"

/* CLASSES ********************************************************************/
/*------------------------------------------------------------------------------
CLASS:	  MotorDriver
PURPOSE:  
------------------------------------------------------------------------------*/
class MotorDriver
{
public:
  // Local Classes
  // Constructors
  MotorDriver(GPIO pulse_gpio, GPIO dir_gpio, GPIO microstep0, GPIO microstep1, GPIO microstep2);
  // Mutators: non-const operations
  bool MotorCmd(s32 distance_raw, u32 max_speed_raw, u8 microstep_mode);
  // Accessors: const operations
  // Static and friend functions
  // Memory management: copy constructor, destructor, operator=
  ~ MotorDriver();
private:
  // Local Classes
  // Constructors
  // Mutators: non-const operations
  // Accessors: const operations
  // Static and friend functions
  // Memory management
  MotorDriver(const  MotorDriver&);
  MotorDriver& operator=(const  MotorDriver& rhs);
  // Data fields
  struct STEPPER_SETUP m_motor_control;
  int m_motor_fd;
  int m_distance_raw;
  int m_max_speed_raw;
  int m_motor_microstep_mode;
  // Static (shared) class variables
};
/* GLOBALS ********************************************************************/

#endif /* MOTORDRIVER_H */
