/*******************************************************************************
FILE:          TestModLegs.cpp
PURPOSE:
*******************************************************************************/

/* INCLUDE ********************************************************************/
#include <iostream>
#include "Config.h"

#include "TestModLegs.h"

/* METHODS ********************************************************************/
/*------------------------------------------------------------------------------
FUNCTION: TestModLegs::TestModLegs
PURPOSE:  Use legs constants to create a starting point
------------------------------------------------------------------------------*/
TestModLegs::TestModLegs(Legs* legs)
  : m_rl_hip ((L_MIN_PULSE_HIP+L_MAX_PULSE_HIP)/2),
    m_rl_knee((L_MIN_PULSE_KNEE+L_MAX_PULSE_KNEE)/2),
    m_ll_hip ((R_MIN_PULSE_HIP+R_MAX_PULSE_HIP)/2),
    m_ll_knee((R_MIN_PULSE_KNEE+R_MAX_PULSE_KNEE)/2)
{
  m_legs = legs;
  using namespace std::placeholders; // for `_1`
  AddIncDec(std::bind(&TestModLegs::Set_RL_Hip,  this, _1), "Set_RL_Hip");
  AddIncDec(std::bind(&TestModLegs::Set_RL_Knee, this, _1), "Set_RL_Knee");
  AddIncDec(std::bind(&TestModLegs::Set_LL_Hip,  this, _1), "Set_LL_Hip");
  AddIncDec(std::bind(&TestModLegs::Set_LL_Knee, this, _1), "Set_LL_Knee");
}

TestModLegs::~TestModLegs()
{
}

/*------------------------------------------------------------------------------
FUNCTION: TestModLegs::callbacks(int); (Right Leg)
------------------------------------------------------------------------------*/
void TestModLegs::Set_RL_Hip(int val)
{
  m_rl_hip += val;

  m_legs->Set_RL_Hip(m_rl_hip);

  cout << "Set_RL_Hip(" << m_rl_hip << ")" << std::endl;
}

void TestModLegs::Set_RL_Knee(int val)
{
  m_rl_knee += val;

  m_legs->Set_RL_Knee(m_rl_knee);
  cout << "Set_RL_Knee(" << m_rl_knee << ")" << std::endl;
}

/*------------------------------------------------------------------------------
FUNCTION: TestModLegs::callbacks(int); (Left Leg)
------------------------------------------------------------------------------*/
void TestModLegs::Set_LL_Hip(int val)
{
  m_ll_hip += val;

  m_legs->Set_LL_Hip(m_ll_hip);
  cout << "Set_LL_Hip(" << m_ll_hip << ")" << std::endl;
}

void TestModLegs::Set_LL_Knee(int val)
{
  m_ll_knee += val;

  m_legs->Set_LL_Knee(m_ll_knee);

  cout << "Set_LL_Knee(" << m_ll_knee << ")" << std::endl;
}
