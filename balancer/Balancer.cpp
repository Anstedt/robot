#include <iostream>

#include "Balancer.h"
#include <functional>

using namespace std;

/* CLASSES ********************************************************************/
/* GLOBALS ********************************************************************/
/* FUNCTIONS ******************************************************************/
/* METHODS ********************************************************************/
/*------------------------------------------------------------------------------
FUNCTION:      Balancer::Balancer()
------------------------------------------------------------------------------*/
Balancer::Balancer()
{
  MotorModeGPIO mode_array = {14,15,18};
      
  m_motor = new Motor(200, 21, 20, mode_array, 5); // Motor uses Gyro data so start it first
  m_motor->Activate();
  
  m_gyro = new Gyro();

  // Resisters Balancer::CallBack(), including passed parameters, with the Gyro
  using namespace std::placeholders; // for `_1, _2, _3, _4`
  m_gyro->RegisterForCallback(std::bind(&Balancer::CallBack, this, _1, _2, _3, _4));
                              
  m_gyro->Activate();

  // Wait for the thread to finish
  m_gyro->JoinThread();
  delete m_gyro;

  m_motor->JoinThread();
  delete m_motor;
}

/*------------------------------------------------------------------------------
FUNCTION:      Balancer::~Balancer()
------------------------------------------------------------------------------*/
Balancer::~Balancer()
{
}

/*------------------------------------------------------------------------------
FUNCTION:      Balancer::CallBack(int pitch, int yaw, float angle_acc, float angle_gyro);
------------------------------------------------------------------------------*/
void Balancer::CallBack(int gyro_yaw_data_raw, int gyro_pitch_data_raw, float angle_gyro, float angle_acc)
{
  cout << "Angle Gyro=" << angle_gyro << "\tAngle Accelerometer=" << angle_acc << "\tGyro Yaw=" << gyro_yaw_data_raw << "\tGyro Pitch=" << gyro_pitch_data_raw << std::endl;
}
