/*******************************************************************************
PACKAGE: Robot

PURPOSE: Creates Gyro and Motors and connecting them up for balancing
*******************************************************************************/
/* INCLUDE ********************************************************************/
#include "Config.h"

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
  // Create a shared mutex for the motor driver
  if (pthread_mutex_init(&m_driver_mutex, NULL) != 0)
  {
    SLOG << "ERROR: Balancer: pthread_mutex_init failed" << std::endl;
  }
  
  // Start Motors
  m_motors = new Motors();

  m_gyro = new Gyro();

  // Registers Balancer::CallBack(), including passed parameters, with the Gyro
  using namespace std::placeholders; // for `_1, _2, _3, _4`

  // This makes sense since Gyro is a thread and needs to call into the
  // balance. Gyro only has a pointer to the method but the method being part of
  // the balancer needs the balancers this pointer.
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

  // Remove Motors
  SLOG << "Balancer Motors" << std::endl;
  delete m_motors;

  SLOG << "~Balancer IS RUNNING gpioTerminate" << std::endl;
  gpioTerminate(); // Now that the MPU6050 is gone we can close pigpio

  // Unlock and destroy mutex so that motors can run to completion
  pthread_mutex_lock(&m_driver_mutex);
}

/*------------------------------------------------------------------------------
FUNCTION: Balancer::CallBack(int pitch, int yaw, float angle_gyro, float angle_acc);

PURPOSE:  See RegisterForCallback() above.
------------------------------------------------------------------------------*/
void Balancer::CallBack(int gyro_pitch, int gyro_yaw, float angle_gyro, float angle_acc)
{
  // SLOG << "Angle Gyro=" << angle_gyro << "\tAngle Accel=" << angle_acc << "\tGyro Pitch=" << gyro_pitch << "\tGyro Yaw=" << gyro_yaw << std::endl;
  // SLOG << "Angle Gyro=" << angle_gyro << "\tAngle Accel=" << angle_acc << std::endl;

  m_motors->AddGyroData(gyro_pitch, gyro_yaw, angle_gyro, angle_acc);
}
