#ifndef MOTOR_H
#define MOTOR_H

/* INCLUDE ********************************************************************/
#include "Threads.h"

/*------------------------------------------------------------------------------
CLASS:	       Motor
PROGRAMMER:    Howard
PURPOSE:       Control a stepper motor
------------------------------------------------------------------------------*/
class Motor
{
public:
  // Local Classes
  // Constructors
  Motor();
  // Mutators: non-const operations
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
  // Static and friend functions
  // Memory management
  Motor(const Motor&);
  Motor& operator=(const Motor& rhs);
  // Data fields
  // Static (shared) class variables
};

#endif /* MOTOR_H */
