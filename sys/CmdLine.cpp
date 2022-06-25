/*******************************************************************************
PACKAGE: System
FILE:    CmdLine.cpp
*******************************************************************************/

/* NOTES ***********************************************************************
*******************************************************************************/

/* INCLUDE ********************************************************************/
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>

#include "Config.h"
#include "CmdLine.h"

#include "Slog.h"

using namespace std;

/* CLASSES ********************************************************************/
/* GLOBALS ********************************************************************/
/* FUNCTIONS ******************************************************************/
/* METHODS ********************************************************************/
/*------------------------------------------------------------------------------
FUNCTION:  CmdLine::CmdLine()
------------------------------------------------------------------------------*/
CmdLine::CmdLine()
{
  m_PID_Kp = PID_Kp;
  m_Kp_set = false;

  m_PID_Ki = PID_Ki;
  m_Ki_set = false;

  m_PID_Kd = PID_Kd;
  m_Kd_set = false;
}

/*------------------------------------------------------------------------------
FUNCTION:  CmdLine::~CmdLine()
------------------------------------------------------------------------------*/
CmdLine::~CmdLine()
{
}

/*------------------------------------------------------------------------------
FUNCTION:  CmdLine::Parse(int argc, char * const argv[], const char *optstring)
PURPOSE:   
------------------------------------------------------------------------------*/
int CmdLine::Parse(int argc, char * const argv[])
{
  int out = 0;
  char c = 'X';
  
  while ((out = getopt (argc, argv, "p:i:d:")) != -1)
  {
    switch (out)
    {
      case 'p':
        m_Kp_set = true;
        m_PID_Kp = atof(optarg);
        break;
      case 'i':
        m_Ki_set = true;
        m_PID_Ki = atof(optarg);
        break;
      case 'd':
        m_Kd_set = true;
        m_PID_Kd = atof(optarg);
        break;
      case '?':
        c = optopt;
        SLOG << "ERROR: Unknown option -" << c << std::endl;        
        return(-1);
      default:
        return(0);
    }
  }
  return(0);
}
