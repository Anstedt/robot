/*******************************************************************************
FILE:    Controller.h

PURPOSE: The robot has a controller for parts of the robot, balancer and legs
*******************************************************************************/
#ifndef CONTROL_H
#define CONTROL_H

/* INCLUDE ********************************************************************/
#include "Balancer.h"

/* CLASSES ********************************************************************/
/*------------------------------------------------------------------------------
CLASS:	  Controller
PURPOSE:  Control the robots balancer, motors and legs.
------------------------------------------------------------------------------*/
class Controller
{
public:
  // Local Classes
  // Constructors
  Controller(double, int, double, double, double);
  bool TestIt();
  // Mutators: non-const operations
  // Accessors: const operations
  // Static and friend functions
  // Memory management: copy constructor, destructor, operator=
  ~Controller();
private:
  // Local Classes
  // Constructors
  // Mutators: non-const operations
  // Accessors: const operations
  // Static and friend functions
  // Memory management
  Controller(const Controller&);
  Controller& operator=(const Controller& rhs);
  // Data fields
  Balancer* m_balancer;
  // Static (shared) class variables
};

#endif /* CONTROL_H */
