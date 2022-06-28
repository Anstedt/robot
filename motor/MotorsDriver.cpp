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

// HJA-TEST
#include <pigpio.h>

#include "Slog.h"

#define STEP_CMD_FILE "/sys/devices/platform/soc/fe20c000.pwm/cmd"

/* METHODS ********************************************************************/

/*------------------------------------------------------------------------------
FUNCTION:  MotorsDriver::MotorsDriver()

PURPOSE:   Setup constants for the motor
------------------------------------------------------------------------------*/
MotorsDriver::MotorsDriver(GPIO m1_pulse_gpio, GPIO m1_dir_gpio, GPIO m2_pulse_gpio, GPIO m2_dir_gpio, GPIO microstep0, GPIO microstep1, GPIO microstep2, pthread_mutex_t* p_driver_mutex)
{
  // Init global for testing
  g_heartbeat_driver = 0;
  
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
FUNCTION:  bool MotorsCmd(m1_speed, m1_distance, m2_speed, m2_distance, mode)
PURPOSE:   Driver motor at the specified rate at the specified speed

ASSUMES:   steps per revolution is 200

ARGUMENTS: mN_speed = pulses per second for motor N
           mN_distance = in pulses for motor N
           mode = stepping mode which is the same for both motors

RETURNS:   worked
------------------------------------------------------------------------------*/
bool MotorsDriver::MotorsCmd(u32 m1_speed, s32 m1_distance, u32 m2_speed, s32 m2_distance, u8 mode)
{
  bool status = true;

  // distance controls motors rotational direction and needs to be the opposite
  // for each motor. The application needs to control this.

  // Speeds the same since we are not ramping
  m_motor_control[0].speed = m1_speed;
  m_motor_control[1].speed = m2_speed;

  // We will shift mode to meet the required speed and distance
  m_motor_control[0].distance =  m1_distance;
  m_motor_control[1].distance =  m2_distance;

  // HJA SLOG << "TEST DEBUG: Motors steps per second=" << m1_speed << ":" << m2_speed << " distance=" << m1_distance << ":" << m2_distance << std::endl;

  // Robot hardware limits us to same mode for both motors
  m_motor_control[0].microstep_control = mode;
  m_motor_control[1].microstep_control = mode;

  uint32_t start = gpioTick();
  uint32_t total = 0;
  
  lseek(m_motor_fd, 0, SEEK_SET); // Start at the beginning

  // SLOG << "ENTER DRIVER m1_speed=" << m1_speed << " m1_distance=" << m1_distance << std::endl;
  if (write(m_motor_fd, &m_motor_control, sizeof(m_motor_control)) != sizeof(m_motor_control))
  {
    SLOG << "ERROR: write to motor driver handle=" << m_motor_fd << " failed" << std::endl;
    status = false;
  }
  g_heartbeat_driver++; // Track all attempt at running the driver
  
  // SLOG << "RETURN DRIVER" << std::endl;

  total = gpioTick() - start;

  if (total > 4000)
    SLOG << "TEST total=" << total << "us speed=" << m1_speed << " distance=" << m1_distance << std::endl;
  
  return(status);
}

/*------------------------------------------------------------------------------
FUNCTION:  bool MotorsCmdSimple(speed, distance, mode)
PURPOSE:   Driver motor at the specified rate at the specified speed

ASSUMES:   both motors go same speed and distance but opposite rotation
           
RETURNS:   worked
------------------------------------------------------------------------------*/
bool MotorsDriver::MotorsCmdSimple(u32 speed, s32 distance, u8 mode)
{
  // Both the motors running the same speed and distance other than direction
  return(MotorsCmd(speed, distance*MOTOR1_DIRECTION, speed, distance*MOTOR2_DIRECTION, mode));
}

/*------------------------------------------------------------------------------
FUNCTION:  MotorsDriver::~MotorsDriver()
------------------------------------------------------------------------------*/
MotorsDriver::~MotorsDriver()
{
  close(m_motor_fd);
}
