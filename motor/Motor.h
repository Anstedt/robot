#ifndef MOTOR_H
#define MOTOR_H

/* INCLUDE ********************************************************************/
#include "Threads.h"
#include "Fifo.h"
#include "Gyro.h"

/*------------------------------------------------------------------------------
CLASS:	       Motor
PROGRAMMER:    Howard
PURPOSE:       Control a stepper motor
------------------------------------------------------------------------------*/
class Motor : public Threads
{
public:
  // Local Classes
  // Constructors
  Motor();
  // Mutators: non-const operations
  bool AddGyroData(int pitch, int yaw, float angle_acc, float angle_gyro);
  // Accessors: const operations
  // Static and friend functions
  // Memory management: copy constructor, destructor, operator=
  ~Motor();
protected:
  int Run(void); // Thread entry point
private:
  // Local Classes
  // Constructors
  // Mutators: non-const operations
  // Accessors: const operations
  // Static and friend functions
  // Memory management
  Motor(const Motor&);
  Motor& operator=(const Motor& rhs);
  // Data fields
  Fifo<float, 512> m_angle_gyro_fifo;
  // Static (shared) class variables
};

#endif /* MOTOR_H */
