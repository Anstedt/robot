/*******************************************************************************
PACKAGE: Robot
FILE:    Gyro.cpp

PURPOSE: Gyroscope Accelorometer Interface
*******************************************************************************/
#include <pigpio.h>
#include <math.h>
#include "Gyro.h"

#include "Config.h"

#include "Slog.h"

using namespace std;

/*------------------------------------------------------------------------------
FUNCTION: Gyro::Gyro()
------------------------------------------------------------------------------*/
Gyro::Gyro()
{
  m_callback = 0;
  m_start = 0;
  m_gyro_Y_data_raw = 0;
  m_gyro_X_data_raw = 0;
  m_angle_acc = 0.0;
  m_angle_gyro = 0.0;

  if (gpioInitialise() < 0)
  {
    SLOG << "Gyro pigpio initialization failed" << std::endl;
  }
  else
  {
    p_mpu6050 = new MPU6050();
  }

  SLOG << "Gyro::Gyro()" <<std::endl;
}

/*------------------------------------------------------------------------------
FUNCTION: Gyro::~Gyro()
------------------------------------------------------------------------------*/
Gyro::~Gyro()
{
  delete p_mpu6050;

  SLOG << "~GYRO NOT RUNNING gpioTerminate" << std::endl;
  // gpioTerminate(); // Now that the MPU6050 is gone we can close pigpio

  SLOG << "Gyro::~Gyro()" <<std::endl;
};

/*------------------------------------------------------------------------------
FUNCTION:      bool Gyro::RegisterForCallback(std::function<void(int)> callback)

Use the C++ functional interface to register a callback with 4 parameters
------------------------------------------------------------------------------*/
bool Gyro::RegisterForCallback(std::function<void(int, int, float, float)> callback)
{
  // We only allow one callback but a vector of callbacks could be used if needed
  if (m_callback == 0)
  {
    m_callback = callback;
  }

  return(true);
}

/*------------------------------------------------------------------------------
FUNCTION: int Gyro::Run(void)

PURPOSE: Process data for Gyro/Accel and send it registered module
------------------------------------------------------------------------------*/
int Gyro::Run(void)
{
  uint32_t timer = 0;
  uint32_t elapsed = 0;

  SLOG << "Gyro:Run() in a separate thread" << std::endl;

  p_mpu6050->set_defaults();

  p_mpu6050->calibrate();

  // Each loop should take 4 ms
  timer = gpioTick() + 4000; // gpioTick is in micro seconds
  elapsed = gpioTick();

  while (ThreadRunning())
  {
    // The 57.296 is the conversions from radians to degrees. The - is so that
    // leaning forward is positive. The 8200(8192) is from the data sheet for
    // AFS_SEL 1.
    m_angle_acc = asin((float)(p_mpu6050->get_accel_Z_cal())/8200.0) * -57.296; //Calculate the current angle according to the accelerometer

    // On startup use accelerometer since that is the best we have
    if(m_start == 0)
    {
      // m_angle_gyro = m_angle_acc; //Load the accelerometer angle in the angle_gyro variable
      m_angle_gyro = p_mpu6050->get_accel_Z_cal_angle(); // Experimental
      m_start = 1; //Set the start variable to start the PID controller
      SLOG << "EXPERIMENTAL m_angle_acc:get_accel_Z_cal=" << m_angle_acc << ":" << p_mpu6050->get_accel_Z_cal() << " VS MPU6050 Cal Z=" << p_mpu6050->get_accel_Z_cal_angle() << std::endl;
    }

    // p_mpu6050->get_gyro_XY(m_gyro_X_data_raw, m_gyro_Y_data_raw);
    m_gyro_X_data_raw = p_mpu6050->get_gyro_X_cal();
    m_gyro_Y_data_raw = p_mpu6050->get_gyro_Y_cal();

    // From data sheet: LSB is 131 LSB/degrees/second and we a reading 250 times per second
    // 0.000031 = 1/(131*250)
    m_angle_gyro += m_gyro_Y_data_raw * 0.000031; //Calculate the traveled during this loop angle and add this to the angle_gyro variable

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //MPU-6050 offset compensation
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Not every gyro is mounted 100% level with the axis of the robot. This can be cause by misalignments during manufacturing of the breakout board.
    //As a result the robot will not rotate at the exact same spot and start to make larger and larger circles.
    //To compensate for this behavior a VERY SMALL angle compensation is needed when the robot is rotating.
    //Try 0.0000003 or -0.0000003 first to see if there is any improvement.

    //Uncomment the following line to make the compensation active
    m_angle_gyro -= m_gyro_X_data_raw * 0.0000003;                  //Compensate the gyro offset when the robot is rotating
    m_angle_gyro = m_angle_gyro * 0.9996 + m_angle_acc * 0.0004;      //Correct the drift of the gyro angle with the accelerometer angle

    if (m_callback)
    {
      m_callback(m_gyro_Y_data_raw, m_gyro_X_data_raw, m_angle_gyro, m_angle_acc);
    }

    // CallBack now has all data
    // SLOG << "Angle Gyro=" << m_angle_gyro << "\tAngle Accelerometer=" << m_angle_acc << "\tGyro X=" << m_gyro_X_data_raw << "\tGyro Y=" << m_gyro_Y_data_raw << std::endl;

    while(timer > gpioTick());
    timer += PRIMARY_THREAD_PERIOD;
  }

  SLOG << "Gyro:Run() DONE in a separate thread : " << (gpioTick() - elapsed) << "us" << std::endl;

  return(ThreadReturn());
}
