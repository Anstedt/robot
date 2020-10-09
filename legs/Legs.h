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
  const int l_chan_hip = 0;
  const int l_min_pulse_hip = 170;
  const int l_max_pulse_hip = 600;
  const int l_chan_knee = 1;
  const int l_min_pulse_knee = 130;
  const int l_max_pulse_knee = 550;

  const int r_chan_hip = 2;
  const int r_min_pulse_hip = 150;
  const int r_max_pulse_hip = 600;
  const int r_chan_knee =3;
  const int r_min_pulse_knee = 150;
  const int r_max_pulse_knee = 600;
  
  // Memory management
  Legs(const Legs&);
  Legs& operator=(const Legs& rhs);
  // Data fields
  Leg m_left;
  Leg m_right;
  // Static (shared) class variables
};
#endif /* LEGS_H */
