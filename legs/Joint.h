/*******************************************************************************
FILE:     Joint.h
PURPOSE:  Legs need joints which are controlled by Servos
*******************************************************************************/

/* NOTES ***********************************************************************
*******************************************************************************/

#ifndef JOINT_H
#define JOINT_H

/* INCLUDE ********************************************************************/
#include "Servo.h"

/* CLASSES ********************************************************************/
/*------------------------------------------------------------------------------
CLASS:	       Joint
------------------------------------------------------------------------------*/
class Joint : public Servo
{
public:
  // Local Classes
  // Constructors
  Joint(int channel, int min_pulse, int max_pulse);
  // Mutators: non-const operations
  bool Stand();
  bool Angle(int angle);
  void SetPWM(int off_pwm);
  // Accessors: const operations
  // Static and friend functions
  // Memory management: copy constructor, destructor, operator=
  ~Joint();
private:
  // Local Classes
  // Constructors
  // Mutators: non-const operations
  // Accessors: const operations
  // Static and friend functions
  // Memory management
  Joint(const Joint&);
  Joint& operator=(const Joint& rhs);
  // Data fields
  const int m_default_freq = 60;
  int m_channel;
  // Static (shared) class variables
};
#endif /* JOINT_H */
