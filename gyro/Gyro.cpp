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
  : m_avgtime(1000), m_heartbeat(0), m_late_max(0)
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
  uint32_t elapsed = 0;

  SLOG << "Gyro:Run() in a separate thread" << std::endl;

  p_mpu6050->set_defaults();

  p_mpu6050->calibrate();

  // Each loop should take 4 ms
  m_timer = gpioTick() + 4000; // gpioTick is in micro seconds
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
    
    RateControlDelay(); // Control Loop Rate
    
    // CallBack now has all data
    // SLOG << "Angle Gyro=" << m_angle_gyro << "\tAngle Accelerometer=" << m_angle_acc << "\tGyro X=" << m_gyro_X_data_raw << "\tGyro Y=" << m_gyro_Y_data_raw << std::endl;
  }
  
  SLOG << "Gyro:Run() DONE in a separate thread : " << (gpioTick() - elapsed) << "us" << std::endl;

  return(ThreadReturn());
}

/*------------------------------------------------------------------------------
FUNCTION:  int Gyro::RateControlDelay()
PURPOSE:   

ARGUMENTS: None
RETURNS:   None
------------------------------------------------------------------------------*/
unsigned int Gyro::RateControlDelay()
{
  uint32_t late = 0;
  uint32_t app_time = 0;
  uint32_t tick = 0;
  uint32_t late_test = 0;
  int late_value = 0;
  
  struct timespec ts = { 0, ((4 % 1000) * 1000 * 1000) };

  // How much time did we use since we got here last, this is only the
  // application time. No we set m_timer so it does not include the sleep
  tick = gpioTick();
  int x = m_timer - tick; // The time it took to get back here
  app_time = abs(x);

  // This averaging time scheme seems to be more stable than all other
  // schemes. It should have some error checking though.
  m_avgtime = (m_avgtime - (m_avgtime/25)) + (app_time/25);
    
  // Sleep subtracting the average time from the thread period we want
  // create a late time, how much more did we sleep than requested
  late_test = gpioTick();
  ts.tv_nsec =  ((PRIMARY_THREAD_PERIOD - m_avgtime) * 1000);
  nanosleep(&ts, NULL);
  late_value = gpioTick() - late_test;
  late = abs(late_value);

  printf("late=%u m_avgtime=%u app_time=%u\n", late, m_avgtime, app_time);
    
  // If all is working we should get back here in 4ms or less. m_timer is set
  // later to be current time, tick, + 4ms (PRIMARY_THREAD_PERIOD).
  if (app_time < 4000)
  {
    // Track how late we are because of gpioDelay() call
    if (late > m_late_max)
    {
      m_late_max = late;
      SLOG << "Track maximum thread delay late_max=" << m_late_max << "us late=" << late << "us app_time=" << app_time << "us"  << std::endl;
    }
  }
  else
  {
    // HJA SLOG << "ERROR: app_time too large app_time=m_timer-tick app_time=" << app_time << " m_timer=" << m_timer << " tick=" << tick << "us late=" << late << "us"<< std::endl;
  } 

  // Print our heartbeat every 4 seconds, 1000/250
  if (m_heartbeat++ > 1000)
  {
    SLOG << "Alive app_time=" << app_time  << "us driver hits=" << g_heartbeat_driver << " late=" << late << "us ts.tv_nsec=" << (ts.tv_nsec / 1000) << "us m_timer=" << m_timer << "us m_angle_gyro=" << m_angle_gyro << " m_avgtime=" << m_avgtime << std::endl;
    m_heartbeat = 0;
    g_heartbeat_driver = 0;
  }
  
  m_timer = gpioTick();
      
  return(late);
}
