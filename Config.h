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

// Motor Constants
const int MOTORS_STEPS_PER_REV = 200;
const int MOTORS_MODE_DEFAULT  = 5;
const int MOTORS_RPM_DEFAULT   = 30;

const int MOTOR1_GPIO_STEP     = 10; // 21
const int MOTOR1_GPIO_DIR      = 18; // 20
const int MOTOR1_GPIO_MODE_0   = 17; // 14
const int MOTOR1_GPIO_MODE_1   = 27; // 15
const int MOTOR1_GPIO_MODE_2   = 22; // 18

const int MOTOR2_GPIO_STEP     = 24;
const int MOTOR2_GPIO_DIR      = 23;
const int MOTOR2_GPIO_MODE_0   = -1;
const int MOTOR2_GPIO_MODE_1   = -1;
const int MOTOR2_GPIO_MODE_2   = -1;

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
