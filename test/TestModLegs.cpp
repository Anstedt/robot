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
  // AddIncDec(Set_RL_Hip, "Set_RL_Hip");
  // AddIncDec(m_legs->Set_RL_Hip, "Set_RL_Hip");
  // AddResults(Get_RL_Hip, "Get_RL_Hip");

  using namespace std::placeholders; // for `_1`
  RegisterForCallback(std::bind(&TestModLegs::CallBack, this, _1), "CallBack");
  RegisterForCallback(std::bind(&TestModLegs::Set_RL_Hip, this, _1), "Set_RL_Hip");
  RegisterForCallback(std::bind(&TestModLegs::Get_RL_Hip, this, _1), "Get_RL_Hip");
}

TestModLegs::~TestModLegs()
{
}


/*------------------------------------------------------------------------------
FUNCTION:      TestModLegs::CallBack(int);
------------------------------------------------------------------------------*/
void TestModLegs::CallBack(int val)
{
  // cout << "Angle Gyro=" << angle_gyro << "\tAngle Accel=" << angle_acc << "\tGyro Pitch=" << gyro_pitch << "\tGyro Yaw=" << gyro_yaw << std::endl;

  m_legs->Set_RL_Hip(val);
  cout << "Set_RL_Hip(" << val << ")" << std::endl;
}

void TestModLegs::Set_RL_Hip(int val)
{
  m_legs->Set_RL_Hip(val);
  cout << "Set_RL_Hip(" << val << ")" << std::endl;
}

void TestModLegs::Get_RL_Hip(int val)
{
  cout << "Get_RL_Hip()" << std::endl;
}
