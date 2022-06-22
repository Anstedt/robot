/*******************************************************************************
PACKAGE: Robot
FILE:    WaitTimer.h

PURPOSE: Interface to wait timer driver 
*******************************************************************************/
#ifndef WAITTIMER_H
#define WAITTIMER_H

/* INCLUDE ********************************************************************/
#include "StandardTypes.h"

#include "rpi4-stepper.h"

/* CLASSES ********************************************************************/
/*------------------------------------------------------------------------------
CLASS:	 WaitTimer

PURPOSE: Interface to wait timer driver
------------------------------------------------------------------------------*/
class WaitTimer
{
public:
  // Local Classes
  // Constructors
  WaitTimer();
  // Mutators: non-const operations
  // Accessors: const operations
  bool Sleep(s32 sleep_in_us);
    // Static and friend functions
  // Memory management: copy constructor, destructor, operator=
  ~WaitTimer();
private:
  // Local Classes
  // Constructors
  // Mutators: non-const operations
  // Accessors: const operations
  // Static and friend functions
  // Memory management
  WaitTimer(const WaitTimer&);
  WaitTimer& operator=(const WaitTimer& rhs);
  // Data fields
  int m_waittimer_fd;
  struct STEPPER_WAIT m_wait;
  // Static (shared) class variables
};

#endif /* WAITTIMER_H */
