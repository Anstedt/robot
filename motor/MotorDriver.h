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
  bool MotorCmd(int steps, int speed);
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
  // Static (shared) class variables
};
/* GLOBALS ********************************************************************/

#endif /* MOTORDRIVER_H */
