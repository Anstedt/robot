/*******************************************************************************
PACKAGE:  Robot
FILE:     MotorsDriver.cpp

PURPOSE:  Low level driver access to motor
*******************************************************************************/

/* NOTES ***********************************************************************
*******************************************************************************/

/* INCLUDE ********************************************************************/
#include <unistd.h>
#include <fcntl.h>
#include "Config.h"

#include "MotorsDriver.h"

#include "Slog.h"

#define STEP_CMD_FILE "/sys/devices/platform/soc/fe20c000.pwm/cmd"

/* METHODS ********************************************************************/

/*------------------------------------------------------------------------------
FUNCTION:  MotorsDriver::MotorsDriver()

PURPOSE:   Setup constants for the motor
------------------------------------------------------------------------------*/
MotorsDriver::MotorsDriver(GPIO m1_pulse_gpio, GPIO m1_dir_gpio, GPIO m2_pulse_gpio, GPIO m2_dir_gpio, GPIO microstep0, GPIO microstep1, GPIO microstep2, pthread_mutex_t* p_driver_mutex)
{
  // Defaults for motor control
  m_motor_control[0].distance  = 0;         // in steps NOTE: signed, if = 0 then stop
  m_motor_control[0].speed = 0;             // min speed in steps, 0 is stopped
	m_motor_control[0].microstep_control = MOTORS_MODE_DEFAULT; // bit 0 is value for gpio_microstep0, bit 1 = microstep1, etc

  m_motor_control[1].distance  = 0;         // in steps NOTE: signed, if = 0 then stop
  m_motor_control[1].speed = 0;             // min speed in steps, 0 is stopped
	m_motor_control[1].microstep_control = MOTORS_MODE_DEFAULT; // bit 0 is value for gpio_microstep0, bit 1 = microstep1, etc

  m_p_driver_mutex = p_driver_mutex;
    
  // Constants for the motor 1 hardware
  m_motor_control[0].gpios[GPIO_STEP]       = m1_pulse_gpio; // constant
  m_motor_control[0].gpios[GPIO_DIRECTION]  = m1_dir_gpio;   // constant
  m_motor_control[0].gpios[GPIO_MICROSTEP0] = microstep0;    // constant
  m_motor_control[0].gpios[GPIO_MICROSTEP1] = microstep1;    // constant
  m_motor_control[0].gpios[GPIO_MICROSTEP2] = microstep2;    // constant

  // Constants for the motor 2 hardware, notice mode lines are the same
  m_motor_control[1].gpios[GPIO_STEP]       = m2_pulse_gpio; // constant
  m_motor_control[1].gpios[GPIO_DIRECTION]  = m2_dir_gpio;   // constant
  m_motor_control[1].gpios[GPIO_MICROSTEP0] = microstep0;    // constant
  m_motor_control[1].gpios[GPIO_MICROSTEP1] = microstep1;    // constant
  m_motor_control[1].gpios[GPIO_MICROSTEP2] = microstep2;    // constant

  m_motor_fd = open(STEP_CMD_FILE, O_RDWR | O_SYNC);  /* might need root access */
	if (m_motor_fd < 0)
  {
		SLOG << "ERROR: could not open motor fd from " << STEP_CMD_FILE << std::endl;
    return;
	}

  lseek(m_motor_fd, 0, SEEK_SET); // Make sure we start at the Begining
}

/*------------------------------------------------------------------------------
FUNCTION:  bool MotorsCmd(s32 distance_raw, u32 max_speed_raw, u8 microstep_mode)
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
bool MotorsDriver::MotorsCmd(s32 distance, u32 speed, u8 microstep_mode)
{
  bool status = true;

  // If not at least =/-4 just set to 0 for driver
  if (distance > -4 && distance < 4)
  {
    distance = 0;
  }
  
  // HJA distance and speed need to be controlled per motor. For pure balancing
  // HJA the will be the same but for turning and moving they will be different
  // HJA remember distance needs to be the opposite for each motor since that
  // HJA controls rotations direction which needs to be the opposite for each
  // HJA motor in most cases.

  // We will shift mode to meet the required speed and distance
  m_motor_control[0].distance =  distance;
  m_motor_control[1].distance =  distance;

  // Speeds the same since we are not ramping
  m_motor_control[0].speed = speed;
  m_motor_control[1].speed = speed;

  SLOG << "DEBUG: Motors steps per second=" << speed << " distance=" << distance << std::endl;

  m_motor_control[0].microstep_control = microstep_mode;
  m_motor_control[1].microstep_control = microstep_mode;

  // Lock and make sure we got the lock
  if (pthread_mutex_lock(m_p_driver_mutex) == 0)
  {
    lseek(m_motor_fd, 0, SEEK_SET); // Start at the beginning

    if (write(m_motor_fd, &m_motor_control, sizeof(m_motor_control)) != sizeof(m_motor_control))
    {
      SLOG << "ERROR: write to motor driver handle=" << m_motor_fd << " failed" << std::endl;
      status = false;
    }
  }
  else
  {
    SLOG << "ERROR: MotorsDriver: pthread_mutex_lock failed" << std::endl;
  }
  
  pthread_mutex_unlock(m_p_driver_mutex);
  
  return(status);
}

/*------------------------------------------------------------------------------
FUNCTION:  MotorsDriver::~MotorsDriver()
------------------------------------------------------------------------------*/
MotorsDriver::~MotorsDriver()
{
}
