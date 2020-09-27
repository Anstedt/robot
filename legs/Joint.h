/*******************************************************************************
FILE:     Joint.h
PURPOSE:  Legs need joints which are controlled by Servos
*******************************************************************************/

/* NOTES ***********************************************************************
*******************************************************************************/

#ifndef JOINT_H
#define JOINT_H

/* INCLUDE ********************************************************************/
#include "Servo.h"

/* CLASSES ********************************************************************/
/*------------------------------------------------------------------------------
CLASS:	       Joint
------------------------------------------------------------------------------*/
class Joint : public Servo
{
public:
  // Local Classes
  // Constructors
  Joint();
  // Mutators: non-const operations
  // Accessors: const operations
  // Static and friend functions
  // Memory management: copy constructor, destructor, operator=
  ~Joint();
private:
  // Local Classes
  // Constructors
  // Mutators: non-const operations
  // Accessors: const operations
  // Static and friend functions
  // Memory management
  Joint(const Joint&);
  Joint& operator=(const Joint& rhs);
  // Data fields
  // Static (shared) class variables
};
#endif /* JOINT_H */
