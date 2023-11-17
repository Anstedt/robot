/*******************************************************************************
PACKAGE: Robot
FILE:    Balancer.h
*******************************************************************************/
#ifndef BALANCER_H
#define BALANCER_H

/* INCLUDE ********************************************************************/
#include "Gyro.h"
#include "Legs.h"
#include "Motors.h"

#include <pthread.h>

/* CLASSES ********************************************************************/
/*------------------------------------------------------------------------------
CLASS:	       Balancer
PURPOSE: Gather data from gyro.
         Use PID to determine required robot position change to balance Control
         motor to get to specified position.
------------------------------------------------------------------------------*/
class Balancer
{
public:
  // Local Classes
  // Constructors
  Balancer(double knee, int offset, double kp, double ki, double kd);
  // Mutators: non-const operations
  // Accessors: const operations
  // Static and friend functions
  // Memory management: copy constructor, destructor, operator=
  ~Balancer();
private:
  // Local Classes
  // Constructors
  // Mutators: non-const operations
  float DynamicAngleCalc(float angle);
  // Accessors: const operations
  // static void* CallBackRun(void* This, int pitch, int yaw, float angle_acc, float angle_gyro);
  // HJA void CallBack(int pitch, int yaw, float angle_acc, float angle_gyro);
  void CallBack(int, int, float, float);
  // Static and friend functions
  const float RANGE_MAX = 8;
  const float RANGE_MIN = -8;
  const unsigned int RANGE_SUM = 25;
  // Memory management
  Balancer(const Balancer&);
  Balancer& operator=(const Balancer& rhs);
  // Data fields
  Gyro* m_gyro;
  Motors* m_motors;
  bool m_motors_running;
  Legs* m_legs;
  float m_max;
  float m_min;
  float m_mid;
  unsigned int m_range;
  bool m_osc_pos;
  bool m_osc_neg;
  
  // Static (shared) class variables
};
/* GLOBALS ********************************************************************/

#endif /* BALANCER_H */
