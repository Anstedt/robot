/*******************************************************************************
PACKAGE:  Robot

NOTE: 90: is 90 degrees forward
       0: is straight down
     -90: is 90 degrees back

PURPOSE:  Hardware based configs for the robot
*******************************************************************************/

#ifndef CONFIG_H
#define CONFIG_H

#include<cmath>

// Converts degrees to radians.
#define DegreesToRadians(angleDegrees) ((angleDegrees) * M_PI / 180.0)

// Converts radians to degrees.
#define RadiansToDegrees(angleRadians) ((angleRadians) * 180.0 / M_PI)

// Globals for testing
extern int g_heartbeat_driver;

// Thread Constants
// The gyro thread drives data all the way to the motors
const unsigned int PRIMARY_THREAD_PERIOD = 4000; // ms
const int PRIMARY_THREAD_RATE = (1000000 / PRIMARY_THREAD_PERIOD); // HZ

// Motor Constants
const int HEARTBEAT = 10; // In seconds

const int MOTORS_STEPS_PER_REV = 200;
const int MOTORS_MODE_DEFAULT  = 5; // 1/32 stepping
const int MOTORS_RPM_DEFAULT   = 30;
// Based on email notes saying I tried 25000 but 3124 average speed would be
// enough. But we need higher speed when we are far from our rotation point
const int MOTORS_MAX_PULSES_PER_SEC = 10000;

// Use this to align max motor speed to max angle
const double MOTOR_ANGLE_RATIO = (MOTORS_MAX_PULSES_PER_SEC/180);

// PID Constants
const double PID_Kp = (MOTOR_ANGLE_RATIO*15);
const double PID_Ki = 20.0;
const double PID_Kd = 30.0;

// Of course right and left legs are reversed as well as the joints

// Left Leg HIP servo constants
const int L_CHAN_HIP       = 0;
const int L_MIN_PULSE_HIP  = 144;
const int L_MAX_PULSE_HIP  = 602;

// Left KNEE servo constants
const int L_CHAN_KNEE      = 1;
const int L_MIN_PULSE_KNEE = 138;
const int L_MAX_PULSE_KNEE = 604;

// Right Leg HIP servo constants
const int R_CHAN_HIP       = 2;
const int R_MIN_PULSE_HIP  = 158;
const int R_MAX_PULSE_HIP  = 618;

// Right KNEE servo constants
const int R_CHAN_KNEE      = 3;
const int R_MIN_PULSE_KNEE = 166;
const int R_MAX_PULSE_KNEE = 632;

// Leg Constants in mm
const int SHIN_LENGTH   = 75;
const int THIGH_LENGTH  = 95;

#endif /* CONFIG_H */
