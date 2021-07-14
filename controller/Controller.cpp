/*******************************************************************************
FILE:    Controller.cpp

PURPOSE: Creates and controls the Balancer/Motor and the Legs
*******************************************************************************/

/* INCLUDE ********************************************************************/
#include "Controller.h"
#include <iostream>
#include <unistd.h>

#include "Slog.h"

#include "TestModLegs.h"

/* METHODS ********************************************************************/
/*------------------------------------------------------------------------------
FUNCTION: Controller::Controller()
------------------------------------------------------------------------------*/
Controller::Controller()
{
  SLOG << "Controller::Controller()" << std::endl;

  m_balancer = new Balancer();
  m_legs = new Legs();

  // This should be in Balancer so it can adjust offset but is fine here for now
  m_legs->Balance(-70, 0); // Knee bent back and wheel offset from robot center

  sleep(4); // Let other threads start 
  
  // This should be in Balancer so it can adjust offset but is fine here for now
  m_legs->Balance(-40, 0); // Knee bent back and wheel offset from robot center
}

/*------------------------------------------------------------------------------
FUNCTION: Controller::~Controller()
------------------------------------------------------------------------------*/
Controller::~Controller()
{
  SLOG << "Controller::~Controller()" << std::endl;

  delete m_balancer;
  delete m_legs;
}
