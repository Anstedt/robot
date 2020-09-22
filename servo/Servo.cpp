#include <iostream>
#include <pigpio.h>
#include "Servo.h"

using namespace std;

/*------------------------------------------------------------------------------
FUNCTION:      Servo::Servo()
ARGUMENTS:     servo_num 0...15
------------------------------------------------------------------------------*/
Servo::Servo()
{
  int mode1;
  
  cout << "Servo::Servo()" << std::endl;

  // RP uses bus 1
  device_fd = i2cOpen(1, Servo::Device_Address, 0);

  set_all_pwm(0, 0);

  WriteByte(Servo::MODE2, Servo::OUTDRV);
  WriteByte(Servo::MODE1, Servo::ALLCALL);
  gpioDelay(0.005 * 1000000); // Micro seconds, wait for oscillator
  
  mode1 = ReadWord(Servo::MODE1);
  mode1 = mode1 & ~SLEEP;  // wake up (reset sleep)
  WriteByte(Servo::MODE1, mode1);

  gpioDelay(0.005 * 1000000); // Micro seconds, wait for oscillator 
}

/*------------------------------------------------------------------------------
FUNCTION: Servo::~Servo()
------------------------------------------------------------------------------*/
Servo::~Servo()
{
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
