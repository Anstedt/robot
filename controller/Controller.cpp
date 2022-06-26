/*******************************************************************************
FILE:    Controller.cpp

PURPOSE: Creates and controls the Balancer/Motor and the Legs
*******************************************************************************/

/* INCLUDE ********************************************************************/
#include <unistd.h>

#include "Controller.h"
#include "Config.h"

#include "CmdLine.h"
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
  // knee angle, wheel offset
  // HJA TEST m_legs->Balance(-90, -30); // Knee bent back and wheel offset from robot center

  sleep(2); // Let other threads start 
  
  // This should be in Balancer so it can adjust offset but is fine here for now

  m_legs->Balance(CmdLine::Instance()->GetKneeAngle(), CmdLine::Instance()->GetLegsOffset()); // Knee bent back and wheel offset from robot center
}

/*------------------------------------------------------------------------------
FUNCTION: Controller::~Controller()
------------------------------------------------------------------------------*/
Controller::~Controller()
{
  SLOG << "Controller::~Controller()" << std::endl;

  delete m_legs;
  delete m_balancer;
}
