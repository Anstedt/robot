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
------------------------------------------------------------------------------*/
Legs::Legs()
  : m_left(L_CHAN_HIP, L_MIN_PULSE_HIP, L_MAX_PULSE_HIP, L_CHAN_KNEE, L_MIN_PULSE_KNEE, L_MAX_PULSE_KNEE),
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

bool Legs::TestIt()
{
  m_left.TestIt();
  m_right.TestIt();
  return(true);
}

void Legs::Set_RL_Hip(int val)
{
  m_right.Set_Hip(val);
}
