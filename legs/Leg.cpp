/*******************************************************************************
FILE:     Leg.cpp
PURPOSE:
*******************************************************************************/

/* NOTES ***********************************************************************
*******************************************************************************/

/* INCLUDE ********************************************************************/
#include "Leg.h"
#include <iostream>

/* METHODS ********************************************************************/
/*------------------------------------------------------------------------------
FUNCTION:  Leg::Leg()
------------------------------------------------------------------------------*/
Leg::Leg(int chan_hip, int min_pulse_hip, int max_pulse_hip, int chan_knee, int min_pulse_knee, int max_pulse_knee)
  : m_hip(chan_hip, min_pulse_hip, max_pulse_hip),
    m_knee(chan_knee, min_pulse_knee, max_pulse_knee)
{
}

/*------------------------------------------------------------------------------
FUNCTION:  Leg::~Leg()
------------------------------------------------------------------------------*/
Leg::~Leg()
{
}

/*------------------------------------------------------------------------------
FUNCTION: bool Leg::Stand()
------------------------------------------------------------------------------*/
bool Leg::Stand()
{
  std::cout << "Leg::Stand()" << std::endl;

  return(m_hip.Stand() && m_knee.Stand());
}

/*------------------------------------------------------------------------------
FUNCTION: bool Leg::Kneel()
------------------------------------------------------------------------------*/
bool Leg::Kneel()
{
  std::cout << "Leg::Kneel()" << std::endl;

  return(m_hip.Angle(0) && m_knee.Angle(180));
}

/*------------------------------------------------------------------------------
FUNCTION: bool Leg::Crouch()
------------------------------------------------------------------------------*/
bool Leg::Crouch()
{
  std::cout << "Leg::Crouch()" << std::endl;

  return(m_hip.Angle(180) && m_knee.Angle(0));
}
