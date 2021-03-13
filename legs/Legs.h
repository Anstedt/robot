/*******************************************************************************
PACKAGE:  Robot
FILE:     Legs.h

PURPOSE:  A robot has 2 legs, each leg has 2 joints/servos
*******************************************************************************/
#ifndef LEGS_H
#define LEGS_H

#include "Leg.h"

/* CLASSES ********************************************************************/
/*------------------------------------------------------------------------------
CLASS:	  Legs
PURPOSE:  Controls the robots legs which has four joints
------------------------------------------------------------------------------*/
class Legs
{
public:
  // Local Classes
  // Constructors
  Legs();
  // Mutators: non-const operations
  bool Stand();
  bool Walk();
  bool Crouch();
  bool Sit();

  bool Balance(double knee_angle, int wheel_offset);

  void Set_RL_Hip(int val)  { m_right.Set_Hip(val); };
  void Set_RL_Knee(int val) { m_right.Set_Knee(val); };
  void Set_LL_Hip(int val)  { m_left.Set_Hip(val); };
  void Set_LL_Knee(int val) { m_left.Set_Knee(val); };
  // Accessors: const operations
  // Static and friend functions
  // Memory management: copy constructor, destructor, operator=
  ~Legs();
private:
  // Local Classes
  // Constructors
  // Mutators: non-const operations
  // Accessors: const operations
  // Static and friend functions
  // Memory management
  Legs(const Legs&);
  Legs& operator=(const Legs& rhs);
  // Data fields
  Leg m_left;
  Leg m_right;
  // Static (shared) class variables
};
#endif /* LEGS_H */
