#include <iostream>

#include "Balancer.h"
#include <functional>

#include <pigpio.h>
#include <unistd.h>
#include <signal.h>

using namespace std;

static sigset_t sigs_to_catch;

/* CLASSES ********************************************************************/
/* GLOBALS ********************************************************************/
/* FUNCTIONS ******************************************************************/
/* METHODS ********************************************************************/
/*------------------------------------------------------------------------------
FUNCTION:      Balancer::Balancer()
------------------------------------------------------------------------------*/
Balancer::Balancer()
{
  int       sig_caught; // signal caught
    
  // Signals turn off control-c, etc
	sigemptyset(&sigs_to_catch);	
	sigaddset(&sigs_to_catch, SIGINT);
  sigaddset(&sigs_to_catch, SIGTERM);
  sigaddset(&sigs_to_catch, SIGSEGV);

  // Block all above signals from all spawned threads 
  if (pthread_sigmask(SIG_BLOCK, &sigs_to_catch, NULL) != 0)
  {
    std::cout << "main::pthread_sigmask() failed" << std::endl;
  }
     
  MotorModeGPIO mode_array = {14,15,18};
      
  m_motor = new Motor(200, 21, 20, mode_array, 5, 30); // Motor uses Gyro data so start it first
  m_motor->Activate(SCHED_FIFO, 1); // Make the motor the highest priority
  
  m_gyro = new Gyro();

  // Resisters Balancer::CallBack(), including passed parameters, with the Gyro
  using namespace std::placeholders; // for `_1, _2, _3, _4`
  m_gyro->RegisterForCallback(std::bind(&Balancer::CallBack, this, _1, _2, _3, _4));
                              
  m_gyro->Activate(SCHED_FIFO, 1);
  
  cout << "Waiting for signal" << std::endl;  
  // But the Balancer thread, main and in our case, waits on those signals to
  // catch them. Wait for a signal to stop, normally control-c
  sigwait(&sigs_to_catch, &sig_caught);
 
  // Now that we got a signal is is time to shutdown
  m_gyro->StopThreadRun();
  while(!m_gyro->ThreadStopped())
  {
    cout << "Waiting for Gyro to stop" << std::endl;
    sleep(1);
  }

  cout << "Balancer Gyro->JoinThread" << std::endl;
  m_gyro->JoinThread();

  cout << "Balancer delete Gyro" << std::endl;
  delete m_gyro;

  cout << "Balancer Motor->JoinThread" << std::endl;
  m_motor->StopThreadRun();
  while(!m_motor->ThreadStopped())
  {
    cout << "Waiting for Motor to stop" << std::endl;
    sleep(1);
  }
  
  m_motor->JoinThread();

  cout << "Balancer delete Motor" << std::endl;
  delete m_motor;
}

/*------------------------------------------------------------------------------
FUNCTION:      Balancer::~Balancer()
------------------------------------------------------------------------------*/
Balancer::~Balancer()
{
  cout << "~Balancer IS RUNNING gpioTerminate" << std::endl;
  gpioTerminate(); // Now that the MPU6050 is gone we can close pigpio
}

/*------------------------------------------------------------------------------
FUNCTION:      Balancer::CallBack(int pitch, int yaw, float angle_gyro, float angle_acc);
------------------------------------------------------------------------------*/
void Balancer::CallBack(int gyro_pitch, int gyro_yaw, float angle_gyro, float angle_acc)
{
  // cout << "Angle Gyro=" << angle_gyro << "\tAngle Accel=" << angle_acc << "\tGyro Pitch=" << gyro_pitch << "\tGyro Yaw=" << gyro_yaw << std::endl;

  m_motor->AddGyroData(gyro_pitch, gyro_yaw, angle_gyro, angle_acc);
}
