/*******************************************************************************
FILE:  Controller.cpp

PURPOSE: Creates and controls the Balancer/Motor and the Legs
*******************************************************************************/

/* NOTES ***********************************************************************
*******************************************************************************/

/* INCLUDE ********************************************************************/
#include "Controller.h"
#include <iostream>

/* METHODS ********************************************************************/
/*------------------------------------------------------------------------------
FUNCTION: Controller::Controller()
------------------------------------------------------------------------------*/
Controller::Controller()
{
  cout << "Controller::Controller()" << std::endl;
  
  m_balancer = new Balancer();
  m_legs = new Legs();
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