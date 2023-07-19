/*******************************************************************************
PACKAGE: Robot
FILE:    Motors.cpp

PURPOSE: Interface to PICO for motor control
*******************************************************************************/

/* NOTES ***********************************************************************
Build and run stand alone tests, a full build is needed first for the threads library
clear ; g++ Motors.cpp -o MotorsTest -I ../sys -I ../ -I . -DMOTORS_TEST -l pigpio -L ../sys -l threads
sudo ./MotorsTest
*******************************************************************************/

/* INCLUDE ********************************************************************/
#include "Motors.h"
#include <cstdio>
#include <unistd.h>
#include <cstring>
#include <pigpio.h>
#include "Config.h"
#include <iostream>

#include "Slog.h"

/* CLASSES ********************************************************************/
/* FUNCTIONS ******************************************************************/
// HJAPID m_pid(&m_input_degrees, &m_output_speed, &m_setpoint, PID_Kp, PID_Ki, PID_Kd, DIRECT)

/*------------------------------------------------------------------------------
FUNCTION:  Motors::Motors()

PURPOSE:   Uses PICO as motor controller
------------------------------------------------------------------------------*/
Motors::Motors():
  m_pid(&m_input_degrees, &m_output_speed, &m_setpoint, PID_Kp, PID_Ki, PID_Kd, DIRECT)
{
  char tty[] = "/dev/ttyACM0";

  printf("Motors()\n");

  m_pid.SetSampleTime(PRIMARY_THREAD_RATE);
  m_pid.SetOutputLimits(-MOTORS_MAX_PULSES_PER_SEC, MOTORS_MAX_PULSES_PER_SEC);
  m_pid.SetMode(AUTOMATIC);

  if (gpioInitialise() < 0)
  {
    printf("gpio init failed\n");
  }

  m_serial = serOpen(tty, 115200, 0);

  if (m_serial >= 0)
    printf("m_serial=%d\n", m_serial);
  else
    printf("ERROR: m_serial=%d", m_serial);
}

/*------------------------------------------------------------------------------
FUNCTION:  Motors::~Motors()
PURPOSE:   
------------------------------------------------------------------------------*/
Motors::~Motors()
{
  printf("~Motors()\n");

  if (m_serial >= 0)
  {
    // HJA send a stop, 0, command to the PICO and maybe sleep a bit to give it
    // time to get it.
    SendCmd(0, 0, 0, 0);
    gpioDelay(4000); // Give PICO 4ms to stop
    serClose(m_serial);
  }

// System does gpioTerminate() so don't do it twice
#ifdef MOTORS_TEST
  gpioTerminate();
#endif
}

/*------------------------------------------------------------------------------
FUNCTION:  bool Motors::AddGyroData(int y, int x, float angle_gyro, float angle_acc)
PURPOSE:   Receives Gyro data, processed data, then sends new command to PICO

ARGUMENTS: None
RETURNS:   None
------------------------------------------------------------------------------*/
bool Motors::AddGyroData(int y, int x, float angle_gyro, float angle_acc)
{
  unsigned int speed; // The pulses per second for the motors, using angle AND mode
  int distance; // Distance based on speed, mode, as well as 250Hz thread rate

  // SLOG << "Angle Gyro=" << angle_gyro << " Angle Accel=" << angle_acc << " Gyro Y=" << y << " Gyro X=" << x << std::endl;

  // Find the speed we need, then the distance
  speed = AngleToSpeed(angle_gyro, &distance);
  
  // Print our heartbeat every 10 seconds, 2500/250
  if (m_heartbeat++ > (PRIMARY_THREAD_RATE*HEARTBEAT))
  {
    SLOG << "Angle Gyro=" << angle_gyro << " speed=" << speed << " distance=" << distance << std::endl;
    m_heartbeat = 0;
  }

  SendCmd(speed, distance, speed, distance); // Send the speed and distance, really direction, to PICO

  return(true);
}

/*------------------------------------------------------------------------------
FUNCTION:  bool Motors::SendCmd(unsigned int speed, int distance)
PURPOSE:   HJA Needs to send commands to PICO

ARGUMENTS:
RETURNS:
------------------------------------------------------------------------------*/
bool Motors::SendCmd(unsigned int m1_speed, int m1_distance, unsigned int m2_speed, int m2_distance)
{
  // Default motors to the same speed
  // HJA Future Adjust speed based on Move and Turn commands
  m_motor1_speed = m1_speed;
  m_motor2_speed = m2_speed;

  // Direction will be 1, 32768 in our case, to indicate forward direction
  // We consider the right side motor on the robot to be the primary motor so
  // We consider distance to indicate forward for + values
  // But the robot wheels need to rotate in opposite directions to go forward,
  // so we modify the left wheels direction to handle this
  
  // Distance controls direction and speed if 0
  if (m1_distance == 0)
    m_motor1_speed = 0;
  else if (m1_distance > 0)
    m_motor1_dir = 32768;  // Forward for motor 1
  else
    m_motor1_dir = 0;

  // HJA maybe we should keep m_motorX_dir in high level mode but ONLY send
  // corrected values to PICO
  // Distance controls direction and speed if 0
  if (m2_distance == 0)
    m_motor2_speed = 0;
  else if (m2_distance > 0)
    m_motor2_dir = 0;  // Forward for motor 2
  else
    m_motor2_dir = 32768;

  // std::cout << "d1=" << m1_distance << " m1s=" << m_motor1_speed << " m1dir=" << m_motor1_dir << std::endl;
  // std::cout << "d2=" << m2_distance << " m2s=" << m_motor2_speed << " m2dir=" << m_motor2_dir << std::endl;
      
  std::string motors = int_to_hex(m_motor1_speed + m_motor1_dir, m_motor2_speed + m_motor2_dir);

  // HJA cast testing int wr = serWrite(m_serial, (char *)motors.c_str(), 11);
  // HJA HJA No need for copy to c_string since (char *)motors.c_str() works fine
  // char* c_string = new char[motors.size() + 1];
  // memcpy(c_string, motors.c_str(), motors.size() + 1);

  // HJA SPEED std::cout << "  motors=" << motors;   // No need since "/n" appended above
  // std::cout << "c_string=" << c_string; // No need since "/n" appended above

  if (m_serial >= 0)
  {
    int wr = serWrite(m_serial, (char *)motors.c_str(), 11);

    // HJA SPEED if (wr != 0)
      // HJA SPEED printf("wr=%d\n", wr);

    // 4000us = 4ms for a rate of 250
    // HJA should be more like 2000, to handle the variability of the 250Hz rate
    gpioDelay(4000); // Give PICO time to respond

    char buf[100];
    
    memset(&buf, '\0', sizeof(buf));
    int rr = serRead(m_serial, buf, sizeof(buf));

    if (m_heartbeat_serial++ > (PRIMARY_THREAD_RATE*HEARTBEAT))
    {
      if (rr >=0)
      {
        printf("%s\n", buf);
        // SLOG << buf << std::endl;
      }
      
      m_heartbeat_serial = 0;
    }
  }
  else
  {
    std::cout << "No Serial Connection" << std::endl;
    
    return(false);
  }

  return(true);
}

/*------------------------------------------------------------------------------
FUNCTION:  unsigned int Motors::AngleToSpeed(float angle, int* distance)
PURPOSE:   HJA Using PID control to adjust motor speed and distance. Maybe
           should be speed and direction and set direction -1/0/1 where 0 also
           changes speed to 0.

ARGUMENTS: None
RETURNS:   None
------------------------------------------------------------------------------*/
unsigned int Motors::AngleToSpeed(float angle, int* distance)
{
  bool dist_reverse = false;
  unsigned int speed = 0;

  m_input_degrees = angle; // This has a range of +/-180
  m_setpoint = 0; // HJA really never changes but we make sure it is set here

  // After everything is setup in the constructor we are ready to go
  m_pid.Compute();

  // In our system speed is always positive, since distance handles direction
  if (m_output_speed < 0)
  {
    m_output_speed *= -1;

    // If the speed was negative distance needs to be negative
    dist_reverse = true;
  }

  speed = m_output_speed;

  // HJA seems like this code is for the old driver
  
  // Make sure the driver has enough distance even if we are 2 periods late
  // *distance = (speed / PRIMARY_THREAD_RATE) * 2;

  // negate distance if speed was negative, note at this point speed is positive
  if (dist_reverse)
    *distance *= -1;

  return(speed);
}

/*------------------------------------------------------------------------------
FUNCTION:  bool Motors::Move(int direction, unsigned int speed)
PURPOSE:   HJA not used yet

ARGUMENTS: None
RETURNS:   None
------------------------------------------------------------------------------*/
bool Motors::Move(unsigned int speed, unsigned int dir)
{
  return(true);
}

/*------------------------------------------------------------------------------
FUNCTION:  bool Motors::Turn(int degrees)
PURPOSE:   HJA not used yet

ARGUMENTS: None
RETURNS:   None
------------------------------------------------------------------------------*/
bool Motors::Turn(int degrees)
{
  return(true);
}

// HJA for testing
#ifdef MOTORS_TEST
int main()
{
  Motors* p_motors = new Motors();

  unsigned int ticks = gpioTick();
  
  for (int i=8000; i>=0; i = i - 1000)
  {
    printf("i=%d\n", i);
    p_motors->SendCmd(i, 1, i*4, 1); // speed, distance
    sleep(5);
  }

  printf("TICKs=%d\n", (gpioTick()-ticks)/1000);  
  
  delete p_motors;
}
#endif

/* METHODS ********************************************************************/
