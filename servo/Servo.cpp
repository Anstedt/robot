#include <iostream>
#include <pigpio.h>
#include "Servo.h"
#include <math.h>

using namespace std;

/*------------------------------------------------------------------------------
FUNCTION:      Servo::Servo()
ARGUMENTS:     servo_num 0...15
------------------------------------------------------------------------------*/
Servo::Servo(int min_pwd, int max_pwd)
{
  int mode1;

  cout << "Servo::Servo()" << std::endl;

  // We only need to calculate the slope if m_min_pwd or m_max_pwd change
  // slope = (output_end - output_start) / (input_end - input_start);
  m_min_pwd = min_pwd;
  m_max_pwd = max_pwd;  
  m_slope_pwd = float(m_max_pwd - m_min_pwd) / float(180 - 0);

  // Should not need this but stops i2cClose from complaining on shutdown
  if (gpioInitialise() < 0)
  {
    cout << "~MPU6050 pigpio initialization failed" << std::endl;
  }

  // RP uses bus 1
  device_fd = i2cOpen(1, Servo::Device_Address, 0);

  set_all_pwm(0, 0);

  WriteByte(Servo::MODE2, Servo::OUTDRV);
  WriteByte(Servo::MODE1, Servo::ALLCALL);
  gpioDelay(0.005 * 1000000); // Micro seconds, wait for oscillator
  
  mode1 = ReadByte(Servo::MODE1);
  cout << "READ mode1=" << mode1 << std::endl;
  mode1 = mode1 & ~SLEEP;  // wake up (reset sleep)
  cout << "mode1=" << mode1 << "~SLEEP=" << ~SLEEP << std::endl;
  WriteByte(Servo::MODE1, mode1);

  gpioDelay(0.005 * 1000000); // Micro seconds, wait for oscillator 
}

/*------------------------------------------------------------------------------
FUNCTION: Servo::~Servo()
------------------------------------------------------------------------------*/
Servo::~Servo()
{
  // Should not need this but stops i2cClose from complaining on shutdown
  if (gpioInitialise() < 0)
  {
    cout << "~MPU6050 pigpio initialization failed" << std::endl;
  }

  i2cClose(device_fd);

  gpioTerminate(); // Now that the MPU6050 is gone we can close pigpio

  cout << "Servo::~Servo()" << std::endl;
}

/*------------------------------------------------------------------------------
FUNCTION:  Servo::set_pwm_freq(int freq_hz)
PURPOSE:       
------------------------------------------------------------------------------*/
void Servo::set_pwm_freq(int freq_hz)
{
  float prescaleval;
  int prescale;
  int oldmode;
  int newmode;
  
  prescaleval = 25000000.0; // 25MHz
  prescaleval /= 4096.0; // 12-bit
  prescaleval /= float(freq_hz);
  prescaleval -= 1.0;

  cout << "After math.floor(prescaleval + 0.5) prescale=" << prescale << std::endl;
  // prescale = int(math.floor(prescaleval + 0.5));
  prescale = int(static_cast<int>(prescaleval + 0.5));
  cout << "After math.floor(prescaleval + 0.5) prescale=" << prescale << std::endl;
  oldmode = ReadByte(Servo::MODE1);
  newmode = (oldmode & 0x7F) | 0x10; // sleep

  WriteByte(Servo::MODE1, newmode); //  go to sleep
  WriteByte(Servo::PRESCALE, prescale);
  WriteByte(Servo::MODE1, oldmode);
  gpioDelay(0.005 * 1000000); // Micro seconds, wait for oscillator

  WriteByte(Servo::MODE1, oldmode | 0x80);
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

  set_pwm(channel, 0, output);

  return(success);
}

/*------------------------------------------------------------------------------
FUNCTION:      Servo::set_pwm(int channel, int on, int off)
------------------------------------------------------------------------------*/
void Servo::set_pwm(int channel, int on, int off)
{
  WriteByte(Servo::LED0_ON_L+4*channel, on & 0xFF);
  WriteByte(Servo::LED0_ON_H+4*channel, on >> 8);
  WriteByte(Servo::LED0_OFF_L+4*channel, off & 0xFF);
  WriteByte(Servo::LED0_OFF_H+4*channel, off >> 8);
}

/*------------------------------------------------------------------------------
FUNCTION: Servo::set_all_pwm(on, off)
------------------------------------------------------------------------------*/
void Servo::set_all_pwm(int on, int off)
{
  WriteByte(Servo::ALL_LED_ON_L, on & 0xFF);
  WriteByte(Servo::ALL_LED_ON_H, on >> 8);
  WriteByte(Servo::ALL_LED_OFF_L, off & 0xFF);
  WriteByte(Servo::ALL_LED_OFF_H, off >> 8);
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
FUNCTION:  Servo::WriteByte(addr, value)
------------------------------------------------------------------------------*/
void Servo::WriteByte(int addr, int value)
{
  i2cWriteByteData(device_fd, addr, value);
}

/*------------------------------------------------------------------------------
FUNCTION:      Servo::ReadByte(addr)
RETURNS:       None
------------------------------------------------------------------------------*/
int Servo::ReadByte(int addr)
{
  return(i2cReadByteData(device_fd, addr));
}

/*------------------------------------------------------------------------------
FUNCTION: int Servo::ReadWord(int addr)
------------------------------------------------------------------------------*/
int Servo::ReadWord(int addr)
{
  return(i2cReadWordData(device_fd, addr));
}
