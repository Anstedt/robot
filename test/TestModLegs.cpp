/*******************************************************************************
FILE:          TestModLegs.cpp
PURPOSE:       
*******************************************************************************/

/* INCLUDE ********************************************************************/
#include <iostream>

#include "TestModLegs.h"

/* METHODS ********************************************************************/
TestModLegs::TestModLegs(Legs* legs)
{
  m_legs = legs;
  using namespace std::placeholders; // for `_1`
  AddIncDec(std::bind(&TestModLegs::Set_RL_Hip,  this, _1), "Set_RL_Hip");
  // AddIncDec(std::bind(&TestModLegs::Get_RL_Hip,  this, _1), "Get_RL_Hip");
  AddIncDec(std::bind(&TestModLegs::Set_RL_Knee, this, _1), "Set_RL_Knee");
  // AddIncDec(std::bind(&TestModLegs::Get_RL_Knee, this, _1), "Get_RL_Knee");

  AddIncDec(std::bind(&TestModLegs::Set_LL_Hip,  this, _1), "Set_LL_Hip");
  // AddIncDec(std::bind(&TestModLegs::Get_LL_Hip,  this, _1), "Get_LL_Hip");
  AddIncDec(std::bind(&TestModLegs::Set_LL_Knee, this, _1), "Set_LL_Knee");
  // AddIncDec(std::bind(&TestModLegs::Get_LL_Knee, this, _1), "Get_LL_Knee");
}

TestModLegs::~TestModLegs()
{
}

/*------------------------------------------------------------------------------
FUNCTION: TestModLegs::callbacks(int); (Right Leg)
------------------------------------------------------------------------------*/
void TestModLegs::Set_RL_Hip(int val)
{
  int cur_val = m_legs->Get_RL_Hip(0);

  m_legs->Set_RL_Hip(cur_val + val);
  cout << "Set_RL_Hip(" << cur_val << ")" << std::endl;
}

void TestModLegs::Get_RL_Hip(int ret)
{
  ret = m_legs->Get_RL_Hip(ret);
  cout << "Get_RL_Hip(" << ret << ")" << std::endl;
}

void TestModLegs::Set_RL_Knee(int val)
{
  int cur_val = m_legs->Get_RL_Knee(val);

  m_legs->Set_RL_Knee(cur_val + val);
  cout << "Set_RL_Knee(" << cur_val << ")" << std::endl;
}

void TestModLegs::Get_RL_Knee(int ret)
{
  ret = m_legs->Get_RL_Knee(ret);
  cout << "Get_RL_Knee(" << ret << ")" << std::endl;
}

/*------------------------------------------------------------------------------
FUNCTION: TestModLegs::callbacks(int); (Left Leg)
------------------------------------------------------------------------------*/
void TestModLegs::Set_LL_Hip(int val)
{
  int cur_val = m_legs->Get_LL_Hip(val);

  m_legs->Set_LL_Hip(cur_val + val);
  cout << "Set_LL_Hip(" << cur_val << ")" << std::endl;
}

void TestModLegs::Get_LL_Hip(int ret)
{
  ret = m_legs->Get_LL_Hip(ret);
  cout << "Get_LL_Hip(" << ret << ")" << std::endl;
}

void TestModLegs::Set_LL_Knee(int val)
{
  int cur_val = m_legs->Get_LL_Knee(val);

  m_legs->Set_LL_Knee(cur_val + val);
  cout << "Set_LL_Knee(" << cur_val << ")" << std::endl;
}

void TestModLegs::Get_LL_Knee(int ret)
{
  ret = m_legs->Get_LL_Knee(ret);
  cout << "Get_LL_Knee(" << ret << ")" << std::endl;
}
