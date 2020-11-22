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
