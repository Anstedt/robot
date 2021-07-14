/*******************************************************************************
PACKAGE: Robot

PURPOSE: Creates Gyro and Motors and connecting them up for balancing
*******************************************************************************/
/* INCLUDE ********************************************************************/
#include "Config.h"
#include <iostream>

#include "Balancer.h"
#include <functional>

#include <pigpio.h>
#include <unistd.h>

#include "Slog.h"

using namespace std;

/* METHODS ********************************************************************/
/*------------------------------------------------------------------------------
FUNCTION:      Balancer::Balancer()
------------------------------------------------------------------------------*/
Balancer::Balancer()
{
  // Right Motor uses Gyro data so start it first
  m_motorRight = new Motor(MOTORS_STEPS_PER_REV, MOTOR1_GPIO_STEP, MOTOR1_GPIO_DIR,
                           MOTOR1_GPIO_MODE_0, MOTOR1_GPIO_MODE_1, MOTOR1_GPIO_MODE_2,
                           MOTORS_MODE_DEFAULT, MOTORS_RPM_DEFAULT, MOTOR1_DIRECTION);

  m_motorRight->Activate(SCHED_FIFO, 1); // Make the motor the highest priority

  // Left Motor uses Gyro data so start it first
  m_motorLeft = new Motor(MOTORS_STEPS_PER_REV, MOTOR2_GPIO_STEP, MOTOR2_GPIO_DIR,
                          MOTOR2_GPIO_MODE_0, MOTOR2_GPIO_MODE_1, MOTOR2_GPIO_MODE_2,
                          MOTORS_MODE_DEFAULT, MOTORS_RPM_DEFAULT, MOTOR2_DIRECTION);

  m_motorLeft->Activate(SCHED_FIFO, 1); // Make the motor the highest priority

  m_gyro = new Gyro();

  // Registers Balancer::CallBack(), including passed parameters, with the Gyro
  using namespace std::placeholders; // for `_1, _2, _3, _4`
  m_gyro->RegisterForCallback(std::bind(&Balancer::CallBack, this, _1, _2, _3, _4));

  m_gyro->Activate(SCHED_FIFO, 1);
}

/*------------------------------------------------------------------------------
FUNCTION:      Balancer::~Balancer()
------------------------------------------------------------------------------*/
Balancer::~Balancer()
{
  // Now that we got a signal is is time to shutdown
  m_gyro->StopThreadRun();
  while(!m_gyro->ThreadStopped())
  {
    SLOG << "Waiting for Gyro to stop" << std::endl;
    sleep(1);
  }

  SLOG << "Balancer Gyro->JoinThread" << std::endl;
  m_gyro->JoinThread();

  SLOG << "Balancer delete Gyro" << std::endl;
  delete m_gyro;

  // STOP AND REMOVE MOTORS
  
  // Remove Right Motor
  SLOG << "Balancer Right Motor->JoinThread" << std::endl;
  m_motorRight->StopThreadRun();
  while(!m_motorRight->ThreadStopped())
  {
    SLOG << "Waiting for Right Motor to stop" << std::endl;
    sleep(1);
  }

  m_motorRight->JoinThread();

  SLOG << "Balancer delete Right Motor" << std::endl;
  delete m_motorRight;

  // Remove Left Motor
  SLOG << "Balancer Left Motor->JoinThread" << std::endl;
  m_motorLeft->StopThreadRun();
  while(!m_motorLeft->ThreadStopped())
  {
    SLOG << "Waiting for Left Motor to stop" << std::endl;
    sleep(1);
  }

  m_motorLeft->JoinThread();

  SLOG << "Balancer delete Left Motor" << std::endl;
  delete m_motorLeft;

  // MOTORS STOPPED AND REMOVED
  
  SLOG << "~Balancer IS RUNNING gpioTerminate" << std::endl;
  gpioTerminate(); // Now that the MPU6050 is gone we can close pigpio
}

/*------------------------------------------------------------------------------
FUNCTION:      Balancer::CallBack(int pitch, int yaw, float angle_gyro, float angle_acc);
------------------------------------------------------------------------------*/
void Balancer::CallBack(int gyro_pitch, int gyro_yaw, float angle_gyro, float angle_acc)
{
  // SLOG << "Angle Gyro=" << angle_gyro << "\tAngle Accel=" << angle_acc << "\tGyro Pitch=" << gyro_pitch << "\tGyro Yaw=" << gyro_yaw << std::endl;
  // SLOG << "Angle Gyro=" << angle_gyro << "\tAngle Accel=" << angle_acc << std::endl;

  m_motorRight->AddGyroData(gyro_pitch, gyro_yaw, angle_gyro, angle_acc);
  m_motorLeft->AddGyroData(gyro_pitch, gyro_yaw, angle_gyro, angle_acc);
}
