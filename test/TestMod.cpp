/*******************************************************************************
PACKAGE: Test
PURPOSE: Upper level test interface      
*******************************************************************************/

/* INCLUDE ********************************************************************/
#include <iostream>

#include "TestMod.h"

/* METHODS ********************************************************************/
void Funcs::AddFunc(void (*function)())
{
  m_funcs.push_back(function);
}

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
        std::cout << "Increment" << std::endl;
        break;
      case 's':
        // Decrement
        std::cout << "Decrement" << std::endl;
        break;
      case 'a':
        // Previous item
        std::cout << "Previous" << std::endl;
        break;
      case 'd':
        // Next item
        std::cout << "Next" << std::endl;
        break;
      case 'p':
        // Print data
        std::cout << "Print" << std::endl;
        break;
      case 'e':
        // Exit
        std::cout << "Exit" << std::endl;
        process = false;
        break;
    }
  }

  return(true);
}
