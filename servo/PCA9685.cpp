#include <iostream>
#include <pigpio.h>
#include "PCA9685.h"

using namespace std;

/* METHODS ********************************************************************/
/*------------------------------------------------------------------------------
FUNCTION:      PCA9685::PCA9685()
------------------------------------------------------------------------------*/
PCA9685::PCA9685()
{
  int mode1;

  cout << "PCA9685::PCA9685()" << std::endl;

  // Should not need this but stops i2cClose from complaining on shutdown
  if (gpioInitialise() < 0)
  {
    cout << "PCA9685 pigpio initialization failed" << std::endl;
  }

  // RP uses bus 1
  device_fd = i2cOpen(1, PCA9685::Device_Address, 0);

  set_all_pwm(0, 0);

  WriteByte(PCA9685::MODE2, PCA9685::OUTDRV);
  WriteByte(PCA9685::MODE1, PCA9685::ALLCALL);
  gpioDelay(0.005 * 1000000); // Micro seconds, wait for oscillator

  mode1 = ReadByte(PCA9685::MODE1);
  cout << "READ mode1=" << mode1 << std::endl;
  mode1 = mode1 & ~SLEEP;  // wake up (reset sleep)
  cout << "mode1=" << mode1 << "~SLEEP=" << ~SLEEP << std::endl;
  WriteByte(PCA9685::MODE1, mode1);

  gpioDelay(0.005 * 1000000); // Micro seconds, wait for oscillator

  cout << "PCA9685 Set frequency" << std::endl;
  set_pwm_freq(60);
}

/*------------------------------------------------------------------------------
FUNCTION: PCA9685::~PCA9685()
------------------------------------------------------------------------------*/
PCA9685::~PCA9685()
{
    // Should not need this but stops i2cClose from complaining on shutdown
  if (gpioInitialise() < 0)
  {
    cout << "~MPU6050 pigpio initialization failed" << std::endl;
  }

  i2cClose(device_fd);

  gpioTerminate(); // Now that the MPU6050 is gone we can close pigpio

  cout << "PCA9685::~PCA9685()" << std::endl;
}

/*------------------------------------------------------------------------------
FUNCTION:  PCA9685::set_pwm_freq(int freq_hz)
------------------------------------------------------------------------------*/
void PCA9685::set_pwm_freq(int freq_hz)
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
  oldmode = ReadByte(PCA9685::MODE1);
  newmode = (oldmode & 0x7F) | 0x10; // sleep

  WriteByte(PCA9685::MODE1, newmode); //  go to sleep
  WriteByte(PCA9685::PRESCALE, prescale);
  WriteByte(PCA9685::MODE1, oldmode);
  gpioDelay(0.005 * 1000000); // Micro seconds, wait for oscillator

  WriteByte(PCA9685::MODE1, oldmode | 0x80);
}

/*------------------------------------------------------------------------------
FUNCTION:      PCA9685::set_pwm(int channel, int on, int off)
------------------------------------------------------------------------------*/
void PCA9685::set_pwm(int channel, int on, int off)
{
  WriteByte(PCA9685::LED0_ON_L+4*channel, on & 0xFF);
  WriteByte(PCA9685::LED0_ON_H+4*channel, on >> 8);
  WriteByte(PCA9685::LED0_OFF_L+4*channel, off & 0xFF);
  WriteByte(PCA9685::LED0_OFF_H+4*channel, off >> 8);
}

/*------------------------------------------------------------------------------
FUNCTION: PCA9685::set_all_pwm(on, off)
------------------------------------------------------------------------------*/
void PCA9685::set_all_pwm(int on, int off)
{
  WriteByte(PCA9685::ALL_LED_ON_L, on & 0xFF);
  WriteByte(PCA9685::ALL_LED_ON_H, on >> 8);
  WriteByte(PCA9685::ALL_LED_OFF_L, off & 0xFF);
  WriteByte(PCA9685::ALL_LED_OFF_H, off >> 8);
}

/*------------------------------------------------------------------------------
FUNCTION:  PCA9685::WriteByte(addr, value)
------------------------------------------------------------------------------*/
void PCA9685::WriteByte(int addr, int value)
{
  i2cWriteByteData(device_fd, addr, value);
}

/*------------------------------------------------------------------------------
FUNCTION:      PCA9685::ReadByte(addr)
RETURNS:       None
------------------------------------------------------------------------------*/
int PCA9685::ReadByte(int addr)
{
  return(i2cReadByteData(device_fd, addr));
}

/*------------------------------------------------------------------------------
FUNCTION: int PCA9685::ReadWord(int addr)
------------------------------------------------------------------------------*/
int PCA9685::ReadWord(int addr)
{
  return(i2cReadWordData(device_fd, addr));
}
