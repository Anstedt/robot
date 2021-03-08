/*******************************************************************************
FILE: Legs.cpp
*******************************************************************************/

/* INCLUDE ********************************************************************/
#include "Config.h"
#include "Legs.h"
#include <iostream>

/* METHODS ********************************************************************/
/*------------------------------------------------------------------------------
FUNCTION: Legs::Legs()
0   = Forward
90  = Straight
180 = Back

NOTE: The left leg values are inverted from the right since the servos are
      inverted or opposite sides from each other.
------------------------------------------------------------------------------*/
Legs::Legs()
  : m_left(L_CHAN_HIP, L_MAX_PULSE_HIP, L_MIN_PULSE_HIP, L_CHAN_KNEE, L_MAX_PULSE_KNEE, L_MIN_PULSE_KNEE),
    m_right(R_CHAN_HIP, R_MIN_PULSE_HIP, R_MAX_PULSE_HIP, R_CHAN_KNEE, R_MIN_PULSE_KNEE, R_MAX_PULSE_KNEE)
{
}

/*------------------------------------------------------------------------------
FUNCTION: Legs::~Legs()
------------------------------------------------------------------------------*/
Legs::~Legs()
{
}

/*------------------------------------------------------------------------------
FUNCTION: bool Legs::Stand()
------------------------------------------------------------------------------*/
bool Legs::Stand()
{
  std::cout << "Legs::Stand()" << std::endl;

  return(m_left.Stand() && m_right.Stand());
}

/*------------------------------------------------------------------------------
FUNCTION: bool Legs::Walk()
------------------------------------------------------------------------------*/
bool Legs::Walk()
{
  std::cout << "Legs::Walk()" << std::endl;

  return(m_left.Walk() && m_right.Walk());
}

/*------------------------------------------------------------------------------
FUNCTION: bool Legs::Kneel()
------------------------------------------------------------------------------*/
bool Legs::Kneel()
{
  std::cout << "Legs::Kneel()" << std::endl;

  return(m_left.Kneel() && m_right.Kneel());
}

/*------------------------------------------------------------------------------
FUNCTION: bool Legs::Crouch()
------------------------------------------------------------------------------*/
bool Legs::Crouch()
{
  std::cout << "Legs::Crouch()" << std::endl;

  return(m_left.Crouch() && m_right.Crouch());
}

/*------------------------------------------------------------------------------
FUNCTION: bool Legs::Sit()
------------------------------------------------------------------------------*/
bool Legs::Sit()
{
  std::cout << "Legs::Sit()" << std::endl;

  return(m_left.Sit() && m_right.Sit());
}
