/*******************************************************************************
PACKAGE: 
FILE:    CmdLine.h

PURPOSE:       
*******************************************************************************/

/* NOTES ***********************************************************************
*******************************************************************************/

#ifndef CMDLINE_H
#define CMDLINE_H

/* INCLUDE ********************************************************************/
#include <getopt.h>

#include "Config.h"

/* CLASSES ********************************************************************/
/*------------------------------------------------------------------------------
CLASS:	 CmdLine

PURPOSE: 
------------------------------------------------------------------------------*/
class CmdLine
{
public:
  // Local Classes
  // Constructors
  static CmdLine* Instance();
  // Mutators: non-const operations
  // Accessors: const operations
  void Help(void);
  inline bool GetKp(double* p) { *p =m_PID_Kp; return(m_Kp_set); }
  inline bool GetKi(double* p) { *p =m_PID_Ki; return(m_Ki_set); }
  inline bool GetKd(double* p) { *p =m_PID_Kd; return(m_Kd_set); }
  inline double GetKneeAngle(void)  { return(m_KneeAngle_set ? m_Legs_KneeAngle : LEGS_KNEE_ANGLE); }
  inline double GetLegsOffset(void) { return(m_Offset_set    ? m_Legs_Offset    : LEGS_OFFSET); }
  // Static and friend functions
  int Parse(int argc, char * const argv[]);
  // Memory management: copy constructor, destructor, operator=
  ~CmdLine();
private:
  // Local Classes
  // Constructors
  CmdLine();
  // Mutators: non-const operations
  // Accessors: const operations
  // Static and friend functions
  // Memory management
  CmdLine(const CmdLine&);
  CmdLine& operator=(const CmdLine& rhs);
  // Data fields
  // PID
  bool   m_Kp_set;
  double m_PID_Kp;
  bool   m_Ki_set;
  double m_PID_Ki;
  bool   m_Kd_set;
  double m_PID_Kd;

  // Legs
  bool   m_KneeAngle_set;
  double m_Legs_KneeAngle;
  bool   m_Offset_set;
  double m_Legs_Offset;
  
  // Static (shared) class variables
};

/*------------------------------------------------------------------------------
FUNCTION:  inline CmdLine* CmdLine::Instance()
RETURNS:   Pointer to the CmdLine singleton
------------------------------------------------------------------------------*/
inline CmdLine* CmdLine::Instance()
{
  static CmdLine instance;

  return (&instance);
}

#endif /* CMDLINE_H */
