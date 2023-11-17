/*******************************************************************************
FILE:    Controller.cpp

PURPOSE: Creates and controls the Balancer/Motor and the Legs
*******************************************************************************/

/* INCLUDE ********************************************************************/
#include "Controller.h"
#include <unistd.h>

#include "Slog.h"

#include "TestModLegs.h"

/* METHODS ********************************************************************/
/*------------------------------------------------------------------------------
FUNCTION: Controller::Controller()
------------------------------------------------------------------------------*/
Controller::Controller(double knee, int offset, double kp, double ki, double kd)
{
  SLOG << "Controller::Controller()" << std::endl;

  m_balancer = new Balancer(knee, offset, kp, ki, kd);
}

/*------------------------------------------------------------------------------
FUNCTION: Controller::~Controller()
------------------------------------------------------------------------------*/
Controller::~Controller()
{
  SLOG << "Controller::~Controller()" << std::endl;

  delete m_balancer;
}
