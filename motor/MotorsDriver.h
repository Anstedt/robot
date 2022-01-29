/*******************************************************************************
PACKAGE: Robot
FILE:    MotorsDriver.h

PURPOSE: Low level motor control interface
*******************************************************************************/

/* NOTES ***********************************************************************
*******************************************************************************/

#ifndef MOTORSDRIVER_H
#define MOTORSDRIVER_H

/* INCLUDE ********************************************************************/
#include "StandardTypes.h"
#include "rpi4-stepper.h"

/* CLASSES ********************************************************************/
/*------------------------------------------------------------------------------
CLASS:	  MotorsDriver
PURPOSE:  
------------------------------------------------------------------------------*/
class MotorsDriver
{
public:
  // Local Classes
  // Constructors
  MotorsDriver(GPIO m1_pulse_gpio, GPIO m1_dir_gpio, GPIO m2_pulse_gpio, GPIO m2_dir_gpio, GPIO microstep0, GPIO microstep1, GPIO microstep2, pthread_mutex_t* p_driver_mutex);
  // Mutators: non-const operations
  bool MotorsCmd(s32 distance_raw, u32 max_speed_raw, u8 microstep_mode);
  // Accessors: const operations
  // Static and friend functions
  // Memory management: copy constructor, destructor, operator=
  ~ MotorsDriver();
private:
  // Local Classes
  // Constructors
  // Mutators: non-const operations
  // Accessors: const operations
  // Static and friend functions
  // Memory management
  MotorsDriver(const  MotorsDriver&);
  MotorsDriver& operator=(const  MotorsDriver& rhs);
  // Data fields
  struct STEPPER_SETUP m_motor_control;
  int m_motor_fd;
  int m_distance_raw;
  int m_max_speed_raw;
  int m_motor_microstep_mode;
  pthread_mutex_t* m_p_driver_mutex;
  // Static (shared) class variables
};
/* GLOBALS ********************************************************************/

#endif /* MOTORSDRIVER_H */
