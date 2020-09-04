#ifndef BALANCER_H
#define BALANCER_H

/* INCLUDE ********************************************************************/
#include "Gyro.h"
#include "Motor.h"

/* CLASSES ********************************************************************/
/*------------------------------------------------------------------------------
CLASS:	       Balancer
PURPOSE:       Gather data from gyro.
               Use PID to determine required robot position change to balance
               Control motor to get to specified position
------------------------------------------------------------------------------*/
class Balancer
{
public:
  // Local Classes
  // Constructors
  Balancer();
  // Mutators: non-const operations
  // Accessors: const operations
  // Static and friend functions
  // Memory management: copy constructor, destructor, operator=
  ~Balancer();
private:
  // Local Classes
  // Constructors
  // Mutators: non-const operations
  // Accessors: const operations
  // static void* CallBackRun(void* This, int pitch, int yaw, float angle_acc, float angle_gyro);
  // HJA void CallBack(int pitch, int yaw, float angle_acc, float angle_gyro);
  void CallBack(int);
  // Static and friend functions
  // Memory management
  Balancer(const Balancer&);
  Balancer& operator=(const Balancer& rhs);
  // Data fields
  Gyro* m_gyro;
  Motor* m_motor;
  // Static (shared) class variables
};
/* GLOBALS ********************************************************************/

#endif /* BALANCER_H */
