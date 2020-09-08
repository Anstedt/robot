#ifndef MOTOR_H
#define MOTOR_H

/* INCLUDE ********************************************************************/
#include "Threads.h"
#include "Fifo.h"

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
  // int (&mode_gpio)[3];
  Motor(int steps_rev, int pulse_gpio, int dir_gpio, int(&mode_gpio)[3], int motor_mode, int revs_per_min = 1);
  // Mutators: non-const operations
  bool AddGyroData(int pitch, int yaw, float angle_acc, float angle_gyro);
  bool SetMotorMode(int mode);
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
  bool SetMotorMode(int mode)
  // Accessors: const operations
  // Static and friend functions
  // Memory management
  Motor(const Motor&);
  Motor& operator=(const Motor& rhs);
  // Data fields
  int m_motor_steps_rev;
  int m_motor_pulse_gpio;
  int m_motor_dir_gpio;
  int m_motor_mode_gpio[3];
  int m_motor_revs_per_min;
  int m_motor_mode;
  // Motor control
  int m_motor_steps_to_go;
  int m_motor_mode;
  int m_motor_dir;
  int m_pulse_high_us;
  int m_pulse_low_us;
  Fifo<float, 512> m_angle_gyro_fifo;
  // Static (shared) class variables
};

#endif /* MOTOR_H */
