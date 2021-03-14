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

  cout << std::endl << "BEG Balance(30,0) for 6 seconds" << std::endl;
  m_legs->Balance(30, 0);
  sleep(6);
  cout << "END Balance for 6 seconds" << std::endl << std::endl;

  cout << std::endl << "BEG Balance(30,10) for 6 seconds" << std::endl;
  m_legs->Balance(30, 10);
  sleep(6);
  cout << "END Balance for 6 seconds" << std::endl << std::endl;

  // TestModLegs test_legs(m_legs);
  // test_legs.ProcessKeys();

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
