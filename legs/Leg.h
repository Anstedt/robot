/*******************************************************************************
FILE:     Leg.h
PURPOSE:  A robot has 2 legs, each leg has a joint
*******************************************************************************/

/* NOTES ***********************************************************************
*******************************************************************************/

#ifndef LEG_H
#define LEG_H

/* INCLUDE ********************************************************************/
#include "Joint.h"

/* CLASSES ********************************************************************/
/*------------------------------------------------------------------------------
CLASS:	       Leg
PURPOSE:       1 Leg, 2 joint
------------------------------------------------------------------------------*/
class Leg
{
public:
  // Local Classes
  // Constructors
  Leg();
  // Mutators: non-const operations
  // Accessors: const operations
  // Static and friend functions
  // Memory management: copy constructor, destructor, operator=
  ~Leg();
private:
  // Local Classes
  // Constructors
  // Mutators: non-const operations
  // Accessors: const operations
  // Static and friend functions
  // Memory management
  Leg(const Leg&);
  Leg& operator=(const Leg& rhs);
  // Data fields
  Joint m_hip;
  Joint m_knee;
  // Static (shared) class variables
};

#endif /* LEG_H */
