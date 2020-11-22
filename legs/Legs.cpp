/*******************************************************************************
FILE: Legs.cpp
*******************************************************************************/

/* INCLUDE ********************************************************************/
#include "Legs.h"
#include <iostream>

/* METHODS ********************************************************************/
/*------------------------------------------------------------------------------
FUNCTION: Legs::Legs()
------------------------------------------------------------------------------*/
Legs::Legs()
  : m_left(Legs::l_chan_hip, Legs::l_min_pulse_hip, Legs::l_max_pulse_hip, Legs::l_chan_knee, Legs::l_min_pulse_knee, Legs::l_max_pulse_knee),
    m_right(Legs::r_chan_hip, Legs::r_min_pulse_hip, Legs::r_max_pulse_hip, Legs::r_chan_knee, Legs::r_min_pulse_knee, Legs::r_max_pulse_knee)
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

