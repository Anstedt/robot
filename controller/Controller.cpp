/*******************************************************************************
FILE:  Controller.cpp

PURPOSE: Creates and controls the Balancer/Motor and the Legs
*******************************************************************************/

/* NOTES ***********************************************************************
*******************************************************************************/

/* INCLUDE ********************************************************************/
#include "Controller.h"
#include <iostream>
#include <unistd.h>

/* METHODS ********************************************************************/
/*------------------------------------------------------------------------------
FUNCTION: Controller::Controller()
------------------------------------------------------------------------------*/
Controller::Controller()
{
  cout << "Controller::Controller()" << std::endl;
  
  m_balancer = new Balancer();
  m_legs = new Legs();

  cout << "Stand for 6 seconds" << std::endl;
  m_legs->Stand();
  sleep(6);

  cout << "Kneel for 6 seconds" << std::endl;
  m_legs->Kneel();
  sleep(6);
 
  cout << "Crouch for 6 seconds" << std::endl;
  m_legs->Crouch();
  sleep(6);  

  cout << "Stand for 6 seconds" << std::endl;
  m_legs->Stand();
  sleep(6);
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
