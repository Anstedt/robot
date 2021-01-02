/* NOTES ***********************************************************************
*******************************************************************************/

#ifndef LEGS_H
#define LEGS_H

/* INCLUDE ********************************************************************/
#include "Leg.h"
/* CLASSES ********************************************************************/
/*------------------------------------------------------------------------------
CLASS:	       Legs
PURPOSE:       Controls the robots legs which has four joints
------------------------------------------------------------------------------*/
class Legs
{
public:
  // Local Classes
  // Constructors
  Legs();
  // Mutators: non-const operations
  bool Stand();
  bool Kneel();
  bool Crouch();

  void Set_RL_Hip(int val)  { m_right.Set_Hip(val); };
  int  Get_RL_Hip(int val)  { return(m_right.Get_Hip(val)); };
  void Set_RL_Knee(int val) { m_right.Set_Knee(val); };
  int  Get_RL_Knee(int val) { return(m_right.Get_Knee(val)); };

  void Set_LL_Hip(int val)  { m_left.Set_Hip(val); };
  int  Get_LL_Hip(int val)  { return(m_left.Get_Hip(val)); };
  void Set_LL_Knee(int val) { m_left.Set_Knee(val); };
  int  Get_LL_Knee(int val) { return(m_left.Get_Knee(val)); };
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
