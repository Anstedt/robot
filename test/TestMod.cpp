/*******************************************************************************
PACKAGE: Test
PURPOSE: Upper level test interface      
*******************************************************************************/

/* INCLUDE ********************************************************************/
#include "TestMod.h"

/* METHODS ********************************************************************/
/*------------------------------------------------------------------------------
FUNCTION: TestMod::TestMod()
------------------------------------------------------------------------------*/
TestMod::TestMod()
  : m_active(0),
    m_incdec(1)
{
}

/*------------------------------------------------------------------------------
FUNCTION: TestMod::~TestMod()
------------------------------------------------------------------------------*/
TestMod::~TestMod()
{
}

/*------------------------------------------------------------------------------
FUNCTION: TestMod::AddIncDec()
PURPOSE:  Method to add call backs
------------------------------------------------------------------------------*/
bool TestMod::AddIncDec(std::function<void(int)> callback, string desc)
{
  m_callback.push_back(callback);
  
  m_description.push_back(desc);
  std::cout << "SET Description=" << desc << std::endl;
  
  return(true);
}

/*------------------------------------------------------------------------------
FUNCTION: TestMod::ProcessKeys()
PURPOSE:  Methods call funcs based on pressed keys
------------------------------------------------------------------------------*/
bool TestMod::ProcessKeys()
{
  char c = 0;
  bool process = true;

  int incdec[] = {1, 10, 200, 500, 1000};
  int incdec_max = 4;
  int incdec_cnt = 0;

  cout << "--------------------------------------------------------------------------------" << std::endl;
  cout << "w: increment and set value" << std::endl;
  cout << "d: decrement and set value" << std::endl;
  cout << "a: select next item" << std::endl;
  cout << "s: select previous item item" << std::endl;
  cout << "[: decrement incdec value {1, 10, 200, 500, 1000}" << std::endl;
  cout << "]: increment incdec value {1, 10, 200, 500, 1000}" << std::endl;
  cout << "e: exit/quit" << std::endl;
  cout << "--------------------------------------------------------------------------------" << std::endl;

  
  while ( process )
  {
    c = getkey();

    switch (c)
    {
      case 'w':
        // Increment
        // Call ACTIVE IncDec function, needs to pass in +
        m_callback[m_active](m_incdec);
        std::cout << "Description=" << m_description[m_active] << std::endl;   
        break;
      case 's':
        // Decrement
        m_callback[m_active](-m_incdec);
        std::cout << "Description=" << m_description[m_active] << std::endl;
        break;
      case 'a':
        // Previous item, handle wrap
        if (--m_active < 0)
          m_active = m_callback.size() - 1;
        std::cout << "Previous m_active=" << m_active << std::endl;
        break;
      case 'd':
        // Next item, handle wrap
        if (++m_active > (m_callback.size()-1))
          m_active = 0;
        std::cout << "Next m_active=" << m_active << std::endl;
        break;
      case 'p':
        // Print data
        // Print results for ACTIVE Results
        std::cout << "Print" << std::endl;
        break;
      case ']':
        // Change increment
        if (++incdec_cnt > incdec_max)
          incdec_cnt = 0;
        m_incdec = incdec[incdec_cnt];
        std::cout << "m_incdec=" << m_incdec << std::endl;
        break;
      case '[':
        // Change increment
        if (--incdec_cnt < 0)
          incdec_cnt = incdec_max;
        m_incdec = incdec[incdec_cnt];
        std::cout << "m_incdec=" << m_incdec << std::endl;
        break;
      case 'e':
        // Exit
        // Break out of the test
        std::cout << "Exit" << std::endl;
        process = false;
        break;
    }
  }

  return(true);
}

/*
Need access to Joints and ability to send in raw values
I need to get to Servo values to set them:

  void Servo::set_min_max_pwm(int min_pwm, int max_pwm)

Joints need interface for this call,
Legs need interface to Joints call
I need to track min max or be able to request them.
  Add int Servo::get_min_pwm()
  Add int Servo::get_max_pwm()
Add access up the chain.

I think I need little interface functions here that are in the array.
void inc_min()
{
  m_active->set_min_max_pwm(get_min_pwm(), get_max_pwm()+1)
}

void inc_max()
{
  m_active->set_min_max_pwm(get_min_pwm()+1, get_max_pwm())
}

In the case of legs these lists are for all the legs
d indexes to next Func in m_incdec and rotates on overflow, sets ACTIVE func
a indexes to previous Func in m_incdec and rotates on underflow, sets ACTIVE func
w execute the ACTIVE func with a + value, example 1
s execute the ACTIVE func with a - value, example -1
 */
