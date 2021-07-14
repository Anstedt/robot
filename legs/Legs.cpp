/*******************************************************************************
FILE: Legs.cpp
*******************************************************************************/

/* INCLUDE ********************************************************************/
#include "Config.h"
#include "Legs.h"

#include "Slog.h"

/* METHODS ********************************************************************/
/*------------------------------------------------------------------------------
FUNCTION: Legs::Legs()
90  = Forward
0   = Straight
-90 = Back

NOTE: The left leg values are inverted from the right since the servos are
      inverted or opposite sides from each other.
------------------------------------------------------------------------------*/
Legs::Legs()
  : m_left(L_CHAN_HIP, L_MIN_PULSE_HIP, L_MAX_PULSE_HIP, L_CHAN_KNEE, L_MIN_PULSE_KNEE, L_MAX_PULSE_KNEE),
    m_right(R_CHAN_HIP, R_MAX_PULSE_HIP, R_MIN_PULSE_HIP, R_CHAN_KNEE, R_MAX_PULSE_KNEE, R_MIN_PULSE_KNEE)
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
  SLOG << "Legs::Stand()" << std::endl;

  return(m_left.Stand() && m_right.Stand());
}

/*------------------------------------------------------------------------------
FUNCTION: bool Legs::Walk()
------------------------------------------------------------------------------*/
bool Legs::Walk()
{
  SLOG << "Legs::Walk()" << std::endl;

  return(m_left.Walk() && m_right.Walk());
}

/*------------------------------------------------------------------------------
FUNCTION: bool Legs::Crouch()
------------------------------------------------------------------------------*/
bool Legs::Crouch()
{
  SLOG << "Legs::Crouch()" << std::endl;

  return(m_left.Crouch() && m_right.Crouch());
}

/*------------------------------------------------------------------------------
FUNCTION: bool Legs::Sit()
------------------------------------------------------------------------------*/
bool Legs::Sit()
{
  SLOG << "Legs::Sit()" << std::endl;

  return(m_left.Sit() && m_right.Sit());
}
/*------------------------------------------------------------------------------
FUNCTION:  bool Balance(double knee_angle, int wheel_offset)
PURPOSE:   Based on knee angle select hip angle based on wheel offset.
           This is so a know balance point, wheel_offset, can be set based on
           the knee angle.

ARGUMENTS: knee_angle   in degrees
           wheel_offset in MM from center of hip on horizontal plain

RETURNS:   true if all goes well
------------------------------------------------------------------------------*/
bool Legs::Balance(double knee_angle, int wheel_offset)
{
  bool lstatus = true;
  bool rstatus = true;
  
  lstatus = m_left.Balance(knee_angle, wheel_offset);
  rstatus = m_right.Balance(knee_angle, wheel_offset);

  return(lstatus && rstatus);
}
