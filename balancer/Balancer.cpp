/*******************************************************************************
PACKAGE: Robot

PURPOSE: Creates Gyro and Motors and connecting them up for balancing
*******************************************************************************/
/* INCLUDE ********************************************************************/
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
Balancer::Balancer(double kp, double ki, double kd)
{
  // Create a shared mutex for the motor driver
  if (pthread_mutex_init(&m_driver_mutex, NULL) != 0)
  {
    SLOG << "ERROR: Balancer: pthread_mutex_init failed" << std::endl;
  }

  // Default dynamic calculation values
  m_max = RANGE_MAX;
  m_min = RANGE_MIN;
  m_range = 0;
  m_mid = 0;

  m_osc_pos = false;
  m_osc_neg = false;

  // Start Motors
  m_motors = new Motors(kp, ki, kd);

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
FUNCTION:  DynamicAngleCalc()
PURPOSE:   Watch for angle oscillations to determine the true 0 for the angle
           when the robot is balanced.

1. Ignore all angles that are outside of the range where we could find zero. We
   are assuming the manually tuned offsets will get us close to zero.
2. Wait for max and min angles that are in range.
3. Calculate new offset based off of max and min angles
4. Calculate midpoint of max and min angles
5. Compare to saved midpoint, default is 0.
6. If significantly different then store new midpoint
7. Offset angle by the stored midpoint

HJA: PROBLEM

  Calculation would be better if we could also use direction changes
  as a clue as to when the robot is oscillating.

  This design is based off the idea that if the robot is oscillating
  we can find the midpoint of the oscillation to dynamically find the
  real offset.

  The issue is if the robot is traveling in ONE direction, this code
  will assume it is oscillating as long as the angle values are not
  exactly the same.

  As an example with the robot sitting on a workbench with the wheels
  disabled a midpoint will be selected that matches the actual angle
  of the robot and then adjust the current angle to be zero since it
  is not changing.

  But we can watch for motor direction changes based on the calculated
  angle since that directly controls the motor direction

  Then we can run the calculation section for a new mid.


ARGUMENTS: None
RETURNS:   None
------------------------------------------------------------------------------*/
float Balancer::DynamicAngleCalc(float angle)
{
  // Angle is in range for consideration
  if (angle < RANGE_MAX && angle > RANGE_MIN)
  {
    // If oscillating find angles in range
    if (m_osc_pos && m_osc_neg)
    {
      // only log when we enter searching
      if (m_min == RANGE_MIN && m_max == RANGE_MAX)
        SLOG << "We are in oscillation, searching for min and max" << std::endl;
      // Find some angles
      if (m_max == RANGE_MAX)
        m_max = angle;
      else if (m_min == RANGE_MIN)
        m_min = angle;
    }
    
    // If we have new angles
    if(m_min != RANGE_MIN && m_max != RANGE_MAX)
    {
      // SLOG << "m_max =" << m_max << " m_min =" << m_min << std::endl;
      // Make sure max is maximum and min is minimum
      if (m_max < m_min)
      {
        // swap
        float tmax = m_max;
        m_max = m_min;
        m_min = tmax;
      }

      // Consider checking max and min against angle for some number of steps,
      // picking the angle if it is outside our current range of mid and max.
      if (angle < m_min) m_min = angle;
      if (angle > m_max) m_max = angle;

      // Track till we have the required number of points
      if (m_range++ >= RANGE_SUM)
      {
        // Find the midpoint
        m_mid = (m_min + m_max)/2;
        SLOG << "m_mid = " << m_mid << " m_min = " << m_min << " m_max = " << m_max << std::endl;

        // Since we have a mid start over
        m_range = 0; // Restart range check
        m_max = RANGE_MAX;
        m_min = RANGE_MIN;
        m_osc_pos = false;
        m_osc_neg = false;
      }
    }
  }

  // m_mid is the offset from the hard coded offset. Since m_mid
  // defaults to 0 nothing changes until it is set to a new value
  float angle_calc = angle - m_mid;

  if (angle_calc > 0)
  {
    if (!m_osc_pos)
      SLOG << "POS: angle_calc=" << angle_calc << " angle=" << angle << " mid=" << m_mid << std::endl;
    m_osc_pos = true;
  }
  else if (angle_calc < 0)
  {
    if (!m_osc_neg)
      SLOG << "NEG: angle_calc=" << angle_calc << " angle=" << angle << " mid=" << m_mid << std::endl;
    m_osc_neg = true;
  }
  else // If angle_calc is 0 the robot stops so we are not oscillating
  {
    m_osc_pos = false;
    m_osc_neg = false;
  }

  return(angle_calc);
}

/*------------------------------------------------------------------------------
FUNCTION: Balancer::CallBack(int pitch, int yaw, float angle_gyro, float angle_acc);

PURPOSE:  See RegisterForCallback() above.
------------------------------------------------------------------------------*/
void Balancer::CallBack(int gyro_pitch, int gyro_yaw, float angle_gyro, float angle_acc)
{
  // SLOG << "Angle Gyro=" << angle_gyro << "\tAngle Accel=" << angle_acc << "\tGyro Pitch=" << gyro_pitch << "\tGyro Yaw=" << gyro_yaw << std::endl;
  // SLOG << "Angle Gyro=" << angle_gyro << "\tAngle Accel=" << angle_acc << std::endl;

  // The gyro's angle is used to calculated the speed sent to the robot.
  // At this point we can calculate the dynamic offset of that angle

  m_motors->AddGyroData(gyro_pitch, gyro_yaw, DynamicAngleCalc(angle_gyro), angle_acc);
}
