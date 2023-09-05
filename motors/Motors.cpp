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
#include <iostream>
#include "Config.h"

#include "Slog.h"

/* CLASSES ********************************************************************/
/* FUNCTIONS ******************************************************************/
/*------------------------------------------------------------------------------
FUNCTION:  Motors::Motors()

PURPOSE:   Uses PICO as motor controller
------------------------------------------------------------------------------*/
Motors::Motors(double kp, double ki, double kd):
  m_pid(&m_input_degrees, &m_output_speed, &m_setpoint, kp, ki, kd, DIRECT)
{
  char tty[] = "/dev/ttyACM0";

  SLOG << "Motors()" << std::endl;

  m_pid.SetSampleTime(PRIMARY_THREAD_RATE);
  m_pid.SetOutputLimits(-MOTORS_MAX_PULSES_PER_SEC, MOTORS_MAX_PULSES_PER_SEC);
  m_pid.SetMode(AUTOMATIC);

  m_serial = serOpen(tty, 115200, 0);

  if (m_serial >= 0)
    printf("m_serial=%d\n", m_serial);
  else
    printf("ERROR: m_serial=%d\n", m_serial);
}

/*------------------------------------------------------------------------------
FUNCTION:  Motors::~Motors()
PURPOSE:   
------------------------------------------------------------------------------*/
Motors::~Motors()
{
  SLOG << "~Motors()" << std::endl;

  if (m_serial >= 0)
  {
    // Send a stop, 0, command to the PICO, sleep to give it time to get it.
    SendCmd(0, 0, 0, 0);
    gpioDelay(4000); // Give PICO 4ms to stop
    serClose(m_serial);
  }
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
    // std::cout << "Motor turned off search for HJAMOTOROFF" << std::endl;
    m_heartbeat = 0;
  }

  SendCmd(speed, distance, speed, distance); // Send the speed and distance, really direction, to PICO

  return(true);
}

/*------------------------------------------------------------------------------
FUNCTION:  bool Motors::SendCmd(unsigned int speed, int distance)
PURPOSE:   

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
    m_motor1_dir = 0;  // Forward for motor 1
  else
    m_motor1_dir = 32768;

  // HJA maybe we should keep m_motorX_dir in high level mode but ONLY send
  // corrected values to PICO
  // Distance controls direction and speed if 0
  if (m2_distance == 0)
    m_motor2_speed = 0;
  else if (m2_distance > 0)
    m_motor2_dir = 32768;  // Forward for motor 2
  else
    m_motor2_dir = 0;

  // std::cout << "d1=" << m1_distance << " m1s=" << m_motor1_speed << " m1dir=" << m_motor1_dir << std::endl;
  // std::cout << "d2=" << m2_distance << " m2s=" << m_motor2_speed << " m2dir=" << m_motor2_dir << std::endl;
      
  std::string motors = int_to_hex(m_motor1_speed + m_motor1_dir, m_motor2_speed + m_motor2_dir);

  if (m_serial >= 0)
  {
    int wr = serWrite(m_serial, (char *)motors.c_str(), 11);

    // 2000us = 2ms so we are faster than 250 Hz
    gpioDelay(2000); // Give PICO time to respond

    char buf[100];
    
    memset(&buf, '\0', sizeof(buf));
    int rr = serRead(m_serial, buf, sizeof(buf));

#ifndef MOTORS_TEST
    if (m_heartbeat_serial++ > (PRIMARY_THREAD_RATE*HEARTBEAT))
#endif
    {
      // std::cout << "d1=" << m1_distance << " m1s=" << m_motor1_speed << " m1dir=" << m_motor1_dir << std::endl;
      // std::cout << "d2=" << m2_distance << " m2s=" << m_motor2_speed << " m2dir=" << m_motor2_dir << std::endl;

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
PURPOSE:   

ARGUMENTS: None
RETURNS:   None
------------------------------------------------------------------------------*/
unsigned int Motors::AngleToSpeed(float angle, int* distance)
{
  bool dist_reverse = false;
  unsigned int speed = 0;

  // Should we set m_input_degrees to 0 if angle is very small HJA ???
  m_input_degrees = angle; // This has a range of +/-180
  m_setpoint = 0; // never changes but we make sure it is set here

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

  // Notice speed is positive here and distance will be correct for direction below
  *distance = (speed / PRIMARY_THREAD_RATE);
  
  // negate distance if speed was negative, note at this point speed is positive
  if (dist_reverse)
    *distance *= -1;

  return(speed);
}

/*------------------------------------------------------------------------------
FUNCTION:  bool Motors::Move(int direction, unsigned int speed)
PURPOSE:   

ARGUMENTS: None
RETURNS:   None
------------------------------------------------------------------------------*/
bool Motors::Move(unsigned int speed, unsigned int dir)
{
  return(true);
}

/*------------------------------------------------------------------------------
FUNCTION:  bool Motors::Turn(int degrees)
PURPOSE:   

ARGUMENTS: None
RETURNS:   None
------------------------------------------------------------------------------*/
bool Motors::Turn(int degrees)
{
  return(true);
}

// Motor Tests
#ifdef MOTORS_TEST
int main(int argc, char *argv[])
{
  int speed = 0;
  int distance = 1; // default a direction
  int wait = 10;
  
  for(;;)
  {
    switch(getopt(argc, argv, "d:s:w:h")) // note the colon (:) to indicate that 'b' has a parameter and is not a switch
    {
      case 'd': // Motor direction
        printf("switch 'd/direction' specified with the value %s\n", optarg);
        distance = atoi(optarg);
        continue;

      case 's': // Motor speed
        printf("switch 's/speed' specified with the value %s\n", optarg);
        speed = atoi(optarg);
        continue;

      case 'w': // Wait time for testing
        printf("switch 'w/wait/sleep' specified with the value %s\n", optarg);
        wait = atoi(optarg);
        continue;

      case -1:
        break;
    }
    break;
  }
  

  if (gpioInitialise() < 0)
  {
    printf("gpio init failed\n");
  }

  Motors* p_motors = new Motors(0, 0, 0); // Only used with PID control
  
  unsigned int ticks = gpioTick();

  // distance controls direction
  p_motors->SendCmd(speed, distance, speed, distance);
  
  /*
  for (int i=8000; i>=0; i = i - 1000)
  {
    printf("i=%d\n", i);
    p_motors->SendCmd(i, 1, i*4, 1); // speed, distance
    sleep(5);
  }
  */

  sleep(wait);
  
  printf("TICKs=%d\n", (gpioTick()-ticks)/1000);  
  
  delete p_motors;

  gpioTerminate();
}
#endif

/* METHODS ********************************************************************/
