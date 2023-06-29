/*******************************************************************************
PACKAGE: Robot
FILE:    Motors.h

PURPOSE: Interface to PICO for motor control 
*******************************************************************************/

/* NOTES ***********************************************************************
clear ; g++ Motors.cpp -o Motors -l pigpio -I ../sys -I ../ -I .
sudo ./Motors
*******************************************************************************/

#ifndef MOTORS_H
#define MOTORS_H

/* INCLUDE ********************************************************************/
#include "PID_v1.h"

/* CLASSES ********************************************************************/
/*------------------------------------------------------------------------------
CLASS:	 Motors

PURPOSE: Motor control
------------------------------------------------------------------------------*/
class Motors
{
public:
  // Local Classes
  // Constructors
  Motors();
  // Mutators: non-const operations
  bool SendCmd(int speed, int distance);
  bool AddGyroData(int pitch, int yaw, float angle_acc, float angle_gyro);
  bool SetMotorsMode(int mode);
  bool Move(int direction, unsigned int speed);
  bool Turn(int degrees); // Cause robot to rotate, +/- degrees specifies  direction of rotation
  // Accessors: const operations
  unsigned int AngleToSpeed(float angle, int* distance);
  // Static and friend functions
  // Memory management: copy constructor, destructor, operator=
  ~Motors() {};
private:
  // Local Classes
  // Constructors
  // Mutators: non-const operations
  // Accessors: const operations
  // Static and friend functions
  // Memory management
  Motors(const Motors&);
  Motors& operator=(const Motors& rhs);
  // Data fields
  // Input: The variable we're trying to control (double)
  // Output: The variable that will be adjusted by the pid (double)
  // Setpoint: Destination in out case which is 0
  // Kp, Ki, Kd: Tuning Parameters. these affect how the pid will change the output. (double>=0)
  //
  // kP is gain, basically how hard will we correct the error. I think if the
  // gain is 1.0 it works just like the system I have right now. Basically the
  // speed is directly proportional to the distance from 0 degrees we are. We
  // see this that as we sit the robot up the speed instantly decreases. If we
  // lay it down the speed instantly speeds up. I think that a lower gain, lets
  // say 0.5 would mean the speed would more slowly go up till we hit max speed
  // again.

  // Ki is the integration, this will sum the error over time and is used to
  // increase the speed if we are not getting to our endpoint or not getting
  // there fast enough.

  // Kd is the derivative. It helps with overshoot.
  
  // Direction: Either DIRECT or REVERSE. determines which direction the output will move when faced with a given error. DIRECT is most common.
  // PID(input, output, set_point, Kp, Ki, Kd, DIRECT);
  // HJAPID PID m_pid;

  double m_input_degrees, m_output_speed, m_setpoint;
  
  int m_motor1_dir = 0;
  int m_motor2_dir = 0;
  int m_motor1_speed = 0;
  int m_motor2_speed = 0;  
  // Static (shared) class variables
};


#endif /* MOTORS_H */
