#ifndef SERVO_H
#define SERVO_H

/* INCLUDE ********************************************************************/
/*------------------------------------------------------------------------------
CLASS:	  Servo
PURPOSE:  Support the PCA9685 as a servo controller
------------------------------------------------------------------------------*/
class Servo
{
public:
  // Local Classes
  // Constructors
  Servo(int servo_min = Servo::SERVOMIN, int servo_max = Servo::SERVOMAX);
  // Mutators: non-const operations
  void set_min_max_pwm(int min_pwm, int max_pwm);
  bool set_servo_angle(int channel, int degrees);
  void set_servo_pwm(int channel, int pwm);
  int  get_servo_pwm(int channel);
  // Accessors: const operations
  // Static and friend functions
  // Servo Values
  static const int SERVOMIN           = 150; // this is the 'minimum' pulse length count (out of 4096)
  static const int SERVOMAX           = 600; // this is the 'maximum' pulse length count (out of 4096)

  // Memory management: copy constructor, destructor, operator=
  ~Servo();
protected:
  int Run(void); // Thread entry point
private:
  // Local Classes
  // Constructors
  // Mutators: non-const operations
  // Accessors: const operations
  // Static and friend functions
  // Memory management
  Servo(const Servo&);
  Servo& operator=(const Servo& rhs);
  // Data fields
  int m_current_pwm;
  int m_min_pwd;
  int m_max_pwd;
  float m_slope_pwd;
};

#endif /* SERVO_H */
