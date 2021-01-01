/*******************************************************************************
PACKAGE: Test
PURPOSE: Upper level test interface      
*******************************************************************************/

/* INCLUDE ********************************************************************/
#include "TestMod.h"

/* METHODS ********************************************************************/
/*------------------------------------------------------------------------------
FUNCTION: Funcs::AddFunc(void (*function)(), string str)
PURPOSE:  Func class for handling vectors of functions and descriptions       
------------------------------------------------------------------------------*/
void Funcs::AddFunc(void (*function)(int), string str)
{
  m_funcDesc.push_back(FuncDesc {function, str});
}

/*------------------------------------------------------------------------------
FUNCTION: TestMod::Add-=-(void (*function)(), string description)
PURPOSE:  Methods to add Funcs for different command sets
------------------------------------------------------------------------------*/
int TestMod::AddIncDec(void (*function)(int), string description)
{
  m_incdec.AddFunc(function, description);

  return(m_incdec.Size());
}

int TestMod::AddSelect(void (*function)(int), string description)
{
  m_select.AddFunc(function, description);
  
  return(m_select.Size());
}

int TestMod::AddResults(void (*function)(int), string description)
{
  m_results.AddFunc(function, description);

  return(m_results.Size());  
}

bool TestMod::RegisterForCallback(std::function<void(int)> callback, string desc)
{
  m_callback.push_back(callback);
  
  m_description.push_back(desc);
  
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
  
  while ( process )
  {
    c = getkey();

    switch (c)
    {
      case 'w':
        // Increment
        // Call ACTIVE IncDec function, needs to pass in +
        std::cout << "Increment" << std::endl;
        break;
      case 's':
        // Decrement
        // Call ACTIVE IncDec function, needs to pass in +
        std::cout << "Decrement" << std::endl;
        break;
      case 'a':
        // Previous item
        // Set previous ACTIVE item, rotate on underflow
        std::cout << "Previous" << std::endl;
        break;
      case 'd':
        // Next item
        // Set next ACTIVE item, rotate on underflow
        std::cout << "Next" << std::endl;
        break;
      case 'p':
        // Print data
        // Print results for ACTIVE Results
        std::cout << "Print" << std::endl;
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
