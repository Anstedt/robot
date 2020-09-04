#include <iostream>

#include "Balancer.h"

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
  m_motor = new Motor(); // Motor uses Gyro data so start it first
  m_motor->Activate();
  
  m_gyro = new Gyro();

  using namespace std::placeholders; // for `_1`
  m_gyro->RegisterForCallback(std::bind(&Balancer::CallBack, this, _1));
                              
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
// HJA void Balancer::CallBack(int pitch, int yaw, float angle_acc, float angle_gyro)
void Balancer::CallBack(int pitch)
{
  cout << pitch << std::endl;
}
