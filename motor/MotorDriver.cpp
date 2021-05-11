/*******************************************************************************
PACKAGE:  Robot
FILE:     MotorDriver.cpp

PURPOSE:  Low level driver access to motor
*******************************************************************************/

/* NOTES ***********************************************************************
*******************************************************************************/

/* INCLUDE ********************************************************************/
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include "Config.h"

#include "MotorDriver.h"

#define STEP_CMD_FILE "/sys/devices/platform/soc/fe20c000.pwm/cmd"

/* METHODS ********************************************************************/

/*------------------------------------------------------------------------------
FUNCTION:  MotorDriver::MotorDriver()

PURPOSE:   Setup constants for the motor
------------------------------------------------------------------------------*/
MotorDriver::MotorDriver(GPIO pulse_gpio, GPIO dir_gpio, GPIO microstep0, GPIO microstep1, GPIO microstep2)
{
  // Defaults for motor control
  m_motor_control.distance  = 0; // in steps NOTE: signed, pos = DIR pin high, neg = DIR pin low NOTE: if = 0 then stop
  m_motor_control.min_speed = 0; // min speed in steps, 0 is stopped
  m_motor_control.max_speed = 0; // min speed in steps, 0 is stopped
	m_motor_control.microstep_control = 0; // bit 0 is value for gpio_microstep0, bit 1 = microstep1, etc

  // Constants for the hardware
  m_motor_control.gpios[GPIO_STEP] = pulse_gpio;       // constant
  m_motor_control.gpios[GPIO_DIRECTION] = dir_gpio;    // constant
  m_motor_control.gpios[GPIO_MICROSTEP0] = microstep0; // constant
  m_motor_control.gpios[GPIO_MICROSTEP1] = microstep1; // constant
  m_motor_control.gpios[GPIO_MICROSTEP2] = microstep2; // constant

  // Constants our our system
	m_motor_control.ramp_aggressiveness    = MOTORS_RAMP_AGGRESSIVENESS;
  m_motor_control.wait_timeout           = MOTORS_WAIT_TIMEOUT;
  m_motor_control.combine_ticks_per_step = MOTORS_COMBINE_TICKS_PER_STEP;

  m_motor_fd = open(STEP_CMD_FILE, O_RDWR | O_SYNC);  /* might need root access */
	if (m_motor_fd < 0)
  {
		std::cout << "ERROR: could not open motor fd from " << STEP_CMD_FILE << std::endl;
    return;
	}

  lseek(m_motor_fd, 0, SEEK_SET); // Make sure we start at the Begining
}

/*------------------------------------------------------------------------------
FUNCTION:  bool MotorCmd(s32 distance_raw, u32 max_speed_raw, u8 microstep_mode)
PURPOSE:   Driver motor at the specified rate at the specified speed

ASSUMES:   steps per revolution is 200

ARGUMENTS: distance_raw  = pulses
                           real distance depends on microstep_mode
                           steps for motor to rotate 360 degrees, normally 200 
                           circumference of wheel
                             often only concerned with angle of rotation
           max_speed_raw = max pulses per second
                           real speed depends on microstep_mode
                           steps for motor to rotate 360 degrees, normally 200 
                           circumference of wheel
                             often only concerned with angle of rotation

EXAMPLE:

steps = steps to take if chopper mode is 1/32 then steps=64 then
motor steps = 2 or 1/100 of a revolution

RETURNS:   worked
------------------------------------------------------------------------------*/
bool MotorDriver::MotorCmd(s32 distance_raw, u32 max_speed_raw, u8 microstep_mode)
{
  bool status = true;
  
  // We will shift mode to meed the required speed and distance
  m_motor_control.distance =  distance_raw;

  // Speeds the same since we are not ramping
  m_motor_control.min_speed = max_speed_raw;
  m_motor_control.max_speed = max_speed_raw;

  m_motor_control.microstep_control = microstep_mode;

  lseek(m_motor_fd, 0, SEEK_SET); // Start at the beginning

  if (write(m_motor_fd, &m_motor_control, sizeof(m_motor_control)) != sizeof(m_motor_control))
  {
    std::cout << "ERROR: write to motor driver handle=" << m_motor_fd << " failed " <<  std::endl;
    status = false;
  }
  
  return(status);
}

/*------------------------------------------------------------------------------
FUNCTION:  MotorDriver::~MotorDriver()
------------------------------------------------------------------------------*/
MotorDriver::~MotorDriver()
{
}
