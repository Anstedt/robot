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
  Servo();
  // Mutators: non-const operations
  void set_pwm_freq(int freq_hz);
  bool set_servo_angle(int channel, int degrees);
  void set_pwm(int channel, int on, int off);
  void set_all_pwm(int on, int off);
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
  void WriteByte(int addr, int value);
  // Accessors: const operations
  int ReadByte(int addr);
  int ReadWord(int addr);
  // Static and friend functions
  // Memory management
  Servo(const Servo&);
  Servo& operator=(const Servo& rhs);
  int device_fd;
  // Constants
  static const int Device_Address = 0x40;	// Device Address/Identifier for PCA9685
  // Registers
  static const int MODE1              = 0x00;
  static const int MODE2              = 0x01;
  static const int SUBADR1            = 0x02;
  static const int SUBADR2            = 0x03;
  static const int SUBADR3            = 0x04;
  static const int LED0_ON_L          = 0x06;
  static const int LED0_ON_H          = 0x07;
  static const int LED0_OFF_L         = 0x08;
  static const int LED0_OFF_H         = 0x09;
  static const int ALL_LED_ON_L       = 0xFA;
  static const int ALL_LED_ON_H       = 0xFB;
  static const int ALL_LED_OFF_L      = 0xFC;
  static const int ALL_LED_OFF_H      = 0xFD;
  static const int PRESCALE           = 0xFE;

  // Flags
  static const int RESTART            = 0x80;
  static const int SLEEP              = 0x10;
  static const int ALLCALL            = 0x01;
  static const int INVRT              = 0x10;
  static const int OUTDRV             = 0x04;

  // Static (shared) class variables
};

#endif /* SERVO_H */
