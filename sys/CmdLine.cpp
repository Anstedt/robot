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

  m_KneeAngle_set = LEGS_KNEE_ANGLE;
  m_Legs_KneeAngle = false;
  m_Offset_set = LEGS_OFFSET;
  m_Legs_Offset = false;
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
  
  while ((out = getopt (argc, argv, "p:i:d:k:o:h")) != -1)
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
      case 'k':
        m_KneeAngle_set = true;
        m_Legs_KneeAngle = atof(optarg);
        printf("m_Legs_KneeAngle=%f\n", m_Legs_KneeAngle);
        break;
      case 'o':
        m_Offset_set = true;
        m_Legs_Offset = atof(optarg);
        printf("m_Legs_Offset=%f\n", m_Legs_Offset);
        break;
      case 'h':
        Help();
        return(1);
        break;
      case '?':
        c = optopt;
        SLOG << "ERROR: Unknown option -" << c << std::endl;        
        Help();
        return(1);
      default:
        return(0);
    }
  }
  return(0);
}

/*------------------------------------------------------------------------------
FUNCTION:  void CmdLine::Help(void)
------------------------------------------------------------------------------*/
void CmdLine::Help(void)
{
  printf("Usage:  %s options\n", "Robot");
  printf("  -p  PID: Proportional constant\n"
         "  -i  PID: Integral constant\n"
         "  -d  PID: Derivative constant\n"
         "  -k  LEGS: Knee angle, negative values bend back\n"
         "  -o  LEGS: Offset from center of robot\n"
         "  -h  Help: This help\n"
         );

  return;
}
