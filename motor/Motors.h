/*******************************************************************************
PACKAGE:       Robot
FILE:          Motors.h
*******************************************************************************/
#ifndef MOTORS_H
#define MOTORS_H

/* INCLUDE ********************************************************************/
#include "PID_v1.h"
#include "StandardTypes.h"
#include "rpi4-stepper.h"
#include "MotorsDriver.h"

/*------------------------------------------------------------------------------
CLASS:	  Motors
PURPOSE:  Control a stepper motor
------------------------------------------------------------------------------*/
class Motors
{
public:
  // Local Classes
  // Constructors
  Motors(GPIO m1_pulse_gpio, GPIO m1_dir_gpio, GPIO m2_pulse_gpio, GPIO m2_dir_gpio, GPIO mode0, GPIO mode1, GPIO mode2, pthread_mutex_t* p_driver_mutex);
  // Mutators: non-const operations
  bool AddGyroData(int pitch, int yaw, float angle_acc, float angle_gyro);
  bool SetMotorsMode(int mode);
  bool Move(s32 direction, u32 speed);
  bool Turn(int degrees); // Cause robot to rotate, +/- degrees specifies  direction of rotation
  // Accessors: const operations
  // Static and friend functions
  // Memory management: copy constructor, destructor, operator=
  ~Motors();
protected:
  int Run(void); // Thread entry point
private:
  // Local Classes
  // Constructors
  // Mutators: non-const operations
  // Accessors: const operations
  bool DriverRateControl(u32 rate, s32 distance);
  bool ThreadRateControl(u32 rate, s32 distance);
  u32 AngleToSpeed(float angle, s32* distance);
    
  // Static and friend functions
  // Memory management
  Motors(const Motors&);
  Motors& operator=(const Motors& rhs);
  // Data fields

  // Motor control
  u8 m_motor_mode; // In our case this will be the same for both motors
  s32 m_motor1_dir;      // this is 1 or -1 since each motor goes in the opposite direction
  s32 m_motor2_dir;      // this is 1 or -1 since each motor goes in the opposite direction

  // Thread rate control
  int m_thread_speed;
  int m_thread_speed_cnt;

  double m_input_degrees, m_output_speed, m_setpoint;

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

  PID m_pid;
  MotorsDriver m_motorsDriver;

  // Static (shared) class variables
};

#endif /* MOTORS_H */
