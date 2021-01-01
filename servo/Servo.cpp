#include <iostream>
#include "Servo.h"
#include "PCA9685.h"
#include <math.h>

using namespace std;

/*------------------------------------------------------------------------------
FUNCTION:      Servo::Servo()
ARGUMENTS:     servo_num 0...15
------------------------------------------------------------------------------*/
Servo::Servo(int min_pwd, int max_pwd)
{
  cout << "Servo::Servo()" << std::endl;

  // We only need to calculate the slope if m_min_pwd or m_max_pwd change
  // slope = (output_end - output_start) / (input_end - input_start);
  m_min_pwd = min_pwd;
  m_max_pwd = max_pwd;  
  m_slope_pwd = float(m_max_pwd - m_min_pwd) / float(180 - 0);
}

/*------------------------------------------------------------------------------
FUNCTION: Servo::~Servo()
------------------------------------------------------------------------------*/
Servo::~Servo()
{
}

/*------------------------------------------------------------------------------
FUNCTION:      bool Servo::set_servo_angle(channel, degrees)
------------------------------------------------------------------------------*/
bool Servo::set_servo_angle(int channel, int degrees)
{
  bool success = false;
  int output;
  
  // If degrees is in range
  if (degrees >=0 && degrees <= 180)
  {
    // output = output_start + round(slope * (input - input_start))
    output = m_min_pwd + round(m_slope_pwd * (degrees - 0));

    cout << "channel=" << channel << " degrees=" << degrees << " slope=" << m_slope_pwd << " output=" << output << std::endl;
    success = true;
  }
  else
  {
    cout << "Angle =" << degrees << " out of range (0-180)" << std::endl;
  }

  PCA9685::Instance()->set_pwm(channel, 0, output);

  return(success);
}

/*------------------------------------------------------------------------------
FUNCTION: Servo::set_min_max_pwm(int min_pwm, int max_pwm)
------------------------------------------------------------------------------*/
void Servo::set_min_max_pwm(int min_pwm, int max_pwm)
{
  // We only need to calculate the slope if m_min_pwd or m_max_pwd change
  m_min_pwd = min_pwm;
  m_max_pwd = max_pwm;  
  m_slope_pwd = float(m_max_pwd - m_min_pwd) / float(180 - 0);
}

/*------------------------------------------------------------------------------
FUNCTION: Servo::set_servo_pwm(int channel, int off)
0   = minimum on time
off = off time
------------------------------------------------------------------------------*/
void Servo::set_servo_pwm(int channel, int off)
{
  PCA9685::Instance()->set_pwm(channel, 0, off);
}

