/*******************************************************************************
PACKAGE:       Robot
PURPOSE:  Hardware based configs for the robot
*******************************************************************************/

#ifndef CONFIG_H
#define CONFIG_H

const int MOTORS_STEPS_PER_REV = 200;
const int MOTORS_MODE_DEFAULT  = 5;
const int MOTORS_RPM_DEFAULT   = 1;

const int MOTOR1_GPIO_STEP     = 10; // 21
const int MOTOR1_GPIO_DIR      = 23; // 20
const int MOTOR1_GPIO_MODE_0   = 17; // 14
const int MOTOR1_GPIO_MODE_1   = 27; // 15
const int MOTOR1_GPIO_MODE_2   = 22; // 18

const int MOTOR2_GPIO_STEP     = 24;
const int MOTOR2_GPIO_DIR      = 23;
const int MOTOR2_GPIO_MODE_0   = -1;
const int MOTOR2_GPIO_MODE_1   = -1;
const int MOTOR2_GPIO_MODE_2   = -1;



#endif /* CONFIG_H */
