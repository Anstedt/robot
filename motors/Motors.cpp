/*******************************************************************************
PACKAGE: Robot
FILE:    Motors.cpp

PURPOSE: Interface to PICO for motor control
*******************************************************************************/

/* NOTES ***********************************************************************
clear ; g++ Motors.cpp -o MotorsTest -l pigpio -I ../sys -I ../ -I . -DMOTORS_TEST
sudo ./Motors
*******************************************************************************/

/* INCLUDE ********************************************************************/
#include "Motors.h"
#include <cstdio>
#include <unistd.h>
#include <cstring>
#include <pigpio.h>
#include "Config.h"

/* CLASSES ********************************************************************/
/* FUNCTIONS ******************************************************************/
// HJAPID m_pid(&m_input_degrees, &m_output_speed, &m_setpoint, PID_Kp, PID_Ki, PID_Kd, DIRECT)
/*------------------------------------------------------------------------------
FUNCTION:  Motors::Motors()

PURPOSE:   Uses PICO as motor controller
------------------------------------------------------------------------------*/
Motors::Motors()
{
  char tty[] = "/dev/ttyACM0";

  printf("Hello World\n");


  if (gpioInitialise() < 0)
  {
    printf("gpio init failed\n");
  }

  int serial = serOpen(tty, 115200, 0);

  if (serial >= 0)
  {
    printf("serial=%s\n", serial);

    char mystr[] = "x00FFy0F00\n";
    int wr = serWrite(serial, mystr, 11);
    if (wr != 0)
      printf("wr=%d\n", wr);

    sleep(1);

    char buf[100];
    memset(&buf, '\0', sizeof(buf));
    int rr = serRead(serial, buf, sizeof(buf));
    if (rr >=0)
      printf("rr=\%d\n%s", rr, buf);

    serClose(serial);
  }
  else
    printf("ERROR: serial=%s", serial);

  gpioTerminate();
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

  // HJA SLOG << "Angle Gyro=" << angle_gyro << "\tAngle Accel=" << angle_acc << "\tGyro Y=" << y << "\tGyro X=" << x << std::endl;

  // Find the speed we need, then the distance
  speed = AngleToSpeed(angle_gyro, &distance);

  SendCmd(speed, distance); // Send the speed and distance, really direction, to PICO

  return(true);
}

/*------------------------------------------------------------------------------
FUNCTION:  bool Motors::SendCmd(int speed, int distance)
PURPOSE:   HJA Needs to send commands to PICO

ARGUMENTS:
RETURNS:
------------------------------------------------------------------------------*/
bool Motors::SendCmd(int speed, int distance)
{
  int dir = 0;

  if (distance = 0)
    speed = 0;
  if (distance > 0)
    dir = 1;
  else
    dir = 0;

  // Now build strings for PICO
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
  // HJAPID m_pid.Compute();

  // In our system speed is always positive, since distance handles direction
  if (m_output_speed < 0)
  {
    m_output_speed *= -1;

    // If the speed was negative distance needs to be negative
    dist_reverse = true;
  }

  speed = m_output_speed;

  // Make sure the driver has enough distance even if we are 2 periods late
  *distance = (speed / PRIMARY_THREAD_RATE) * 2;

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
bool Motors::Move(int direction, unsigned int speed)
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

  delete p_motors;
}
#endif

/* METHODS ********************************************************************/
