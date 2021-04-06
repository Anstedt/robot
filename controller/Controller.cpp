/*******************************************************************************
FILE:    Controller.cpp

PURPOSE: Creates and controls the Balancer/Motor and the Legs
*******************************************************************************/

/* INCLUDE ********************************************************************/
#include "Controller.h"
#include <iostream>
#include <unistd.h>

#include "TestModLegs.h"

/* METHODS ********************************************************************/
/*------------------------------------------------------------------------------
FUNCTION: Controller::Controller()
------------------------------------------------------------------------------*/
Controller::Controller()
{
  cout << "Controller::Controller()" << std::endl;

  m_balancer = new Balancer();
  m_legs = new Legs();

  TestModLegs test_legs(m_legs);
  test_legs.ProcessKeys();

  // sleep(4); // Let other threads start 

  // cout << std::endl << "Balance(-40, 0)" << std::endl;
  // m_legs->Balance(-40, 0);
  // sleep(6);
  
  // cout << std::endl << "BEG Balance(-40, 10)" << std::endl;
  // m_legs->Balance(-40, 10);
  // sleep(6);
  
  // cout << std::endl << "BEG Balance(-40, -10)" << std::endl;
  // m_legs->Balance(-40, -10);
  // sleep(6);

  /*
  cout << std::endl << "Balance(0, 0)" << std::endl;
  m_legs->Balance(0, 0);
  sleep(6);
  
  cout << std::endl << "BEG Balance(0, 80)" << std::endl;
  m_legs->Balance(0, 80);
  sleep(6);
  
  cout << std::endl << "BEG Balance(0, -80)" << std::endl;
  m_legs->Balance(0, -80);
  sleep(6);
  */
  
  // cout << std::endl << "Balance(30, 0)" << std::endl;
  // m_legs->Balance(30, 0);
  // sleep(6);
  
  // cout << std::endl << "BEG Balance(30, 80)" << std::endl;
  // m_legs->Balance(30, 80);
  // sleep(6);
  
  // cout << std::endl << "BEG Balance(30, -80)" << std::endl;
  // m_legs->Balance(30, -80);
  // sleep(6);

  // cout << std::endl << "Balance(-30, 0)" << std::endl;
  // m_legs->Balance(-30, 0);
  // sleep(6);
  
  // cout << std::endl << "BEG Balance(-30, 80)" << std::endl;
  // m_legs->Balance(-30, 80);
  // sleep(6);
  
  // cout << std::endl << "BEG Balance(-30, -80)" << std::endl;
  // m_legs->Balance(-30, -80);
  // sleep(6);


  // cout << std::endl << "Balance(10, 0)" << std::endl;
  // m_legs->Balance(10, 0);
  // sleep(6);
  
  // cout << std::endl << "BEG Balance(10, 10)" << std::endl;
  // m_legs->Balance(10, 10);
  // sleep(6);
  
  // cout << std::endl << "BEG Balance(10, -10)" << std::endl;
  // m_legs->Balance(10, -10);
  // sleep(6);



  // cout << std::endl << "BEG Balance(80, 0)" << std::endl;
  // m_legs->Balance(80, 0);
  // sleep(6);

  // cout << std::endl << "BEG Balance(80, 10)" << std::endl;
  // m_legs->Balance(80, 10);
  // sleep(6);

  // cout << std::endl << "BEG Balance(80, -10)" << std::endl;
  // m_legs->Balance(80, -10);
  // sleep(6);
  
  /*
  cout << "Walk for 6 seconds" << std::endl;
  m_legs->Walk();
  sleep(6);

  cout << "Crouch for 6 seconds" << std::endl;
  m_legs->Crouch();
  sleep(6);

  cout << "Stand for 6 seconds" << std::endl;
  m_legs->Stand();
  sleep(6);
  */
}

/*------------------------------------------------------------------------------
FUNCTION: Controller::~Controller()
------------------------------------------------------------------------------*/
Controller::~Controller()
{
  cout << "Controller::~Controller()" << std::endl;

  delete m_balancer;
  delete m_legs;
}
