/*******************************************************************************
FILE:     Leg.cpp
PURPOSE:
*******************************************************************************/
/* INCLUDE ********************************************************************/
#include "Leg.h"
#include "Config.h"
#include <iostream>
#include<cmath>

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
FUNCTION:  bool Leg::Stand()
------------------------------------------------------------------------------*/
bool Leg::Stand()
{
  std::cout << "Leg::Stand()" << std::endl;

  return(m_hip.Angle(0) && m_knee.Angle(0));
}

/*------------------------------------------------------------------------------
FUNCTION:  bool Leg::Walk()
------------------------------------------------------------------------------*/
bool Leg::Walk()
{
  std::cout << "Leg::Walk()" << std::endl;

  return(m_hip.Angle(20) && m_knee.Angle(-60));
}

/*------------------------------------------------------------------------------
FUNCTION:  bool Leg::Crouch()
------------------------------------------------------------------------------*/
bool Leg::Crouch()
{
  std::cout << "Leg::Crouch()" << std::endl;

  return(m_hip.Angle(90) && m_knee.Angle(-90));
}

/*------------------------------------------------------------------------------
FUNCTION:  bool Leg::Sit()
------------------------------------------------------------------------------*/
bool Leg::Sit()
{
  std::cout << "Leg::Sit() On Chair" << std::endl;

  return(m_hip.Angle(90) && m_knee.Angle(-90));
}

/*------------------------------------------------------------------------------
FUNCTION:  bool Balance(int knee_angle, int wheel_offset)
PURPOSE:   Based on knee angle select hip angle based on wheel offset.
           This is so a know balance point, wheel_offset, can be set based on
           the knee angle.

ARGUMENTS: knee_angle   in degrees
           wheel_offset in MM from center of hip on horizontal plain

RETURNS:   true if all goes well
------------------------------------------------------------------------------*/
bool Leg::Balance(int knee_angle, int wheel_offset)
{
  int hip_angle = GetHipAngle(knee_angle, wheel_offset);

  m_knee.Angle(knee_angle);
  m_hip.Angle(hip_angle);

  return(true);
}

/*------------------------------------------------------------------------------
FUNCTION:  int GetHipAngle(double knee_angle, int wheel_offset)
PURPOSE:   Based on the selected knee angle and wheel_offset find the hip
           angle

ARGUMENTS: knee_angle   in degrees
           wheel_offset in MM from center of hip on horizontal plain

d = sqrt(a**2 + b**2 2ab*cos(knee))

hip = asin((b/d)*sin(knee) + acos(c/d)

a = hip length
b = shin lenght

RETURNS:   Hip angle in degress
------------------------------------------------------------------------------*/
int Leg::GetHipAngle(double knee_angle, int wheel_offset)
{
  int shin = SHIN_LENGTH;
  int thigh = THIGH_LENGTH;

  int shin2  = std::pow(shin, 2);
  int thigh2 = std::pow(thigh, 2);

  double knee_rad = degreesToRadians(knee_angle);

  double knee_cos = cos(knee_rad);
  
  
  return(0);
}
