/*******************************************************************************
FILE:     Leg.cpp
PURPOSE:
*******************************************************************************/
/* INCLUDE ********************************************************************/
#include "Leg.h"
#include "Config.h"
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
bool Leg::Balance(double knee_angle, double wheel_offset)
{
  double hip_angle = GetHipAngle(knee_angle, wheel_offset);

  m_knee.Angle(knee_angle);
  m_hip.Angle(hip_angle);

  return(true);
}

/*------------------------------------------------------------------------------
FUNCTION:  double GetHipAngle(double knee_angle, double wheel_offset)
PURPOSE:   Based on the selected knee angle and wheel_offset find the hip
           angle

ARGUMENTS: knee_angle   in degrees
           wheel_offset in MM from center of hip on horizontal plain

a = thigh length
b = shin length
c = wheel offset
d = hypotenuse(calculated)

RETURNS:   Hip angle in degress
------------------------------------------------------------------------------*/
double Leg::GetHipAngle(double knee_angle, double wheel_offset)
{
  
  bool invert = true;
  int thigh = THIGH_LENGTH; // a
  int shin = SHIN_LENGTH;   // b

  int thigh2 = std::pow(thigh, 2); // a^2
  int shin2  = std::pow(shin, 2);  // b^2

  // Normally the hip angle is the opposite direction from the knee angle. If
  // the knee angle is positive the hip angle is negative
  if (knee_angle <= 0)
  {
    invert = false;
  }
  
  // Correct for equation angles versus physical angles
  knee_angle = 180 - knee_angle; // Theta 0
  
  double knee_rad = DegreesToRadians(knee_angle);
  
  // calculated the hypotenuse, d = sqrt(a**2 + b**2 2ab*cos(knee))
  double hypotenuse = std::sqrt(shin2 + thigh2 - 2*thigh*shin*cos(knee_rad));

  // Now the hip angle, hip = asin((b/d)*sin(knee) + acos(c/d)
  double hip_angle_rad = asin((shin/hypotenuse)*sin(knee_rad)) + acos(wheel_offset/hypotenuse);

  // No covert to degrees and adjust to physical coordinate system
  double hip_angle = RadiansToDegrees(hip_angle_rad) - 90; // adjust

  // Now adjust based on the original knee angle
  if (invert)
  {
    hip_angle = -hip_angle;
  }

  // Display results
  std::cout << "hip_angle=" << hip_angle << " Shin:" << shin << " Thigh:" << thigh << " Knee:" << knee_angle << " Offset:" << wheel_offset << std::endl;
  
  return(hip_angle);
}
