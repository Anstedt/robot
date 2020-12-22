/*******************************************************************************
PACKAGE: Test
PURPOSE: Upper level test interface      
*******************************************************************************/

/* INCLUDE ********************************************************************/
#include "TestMod.h"

/* METHODS ********************************************************************/

void Funcs::AddFunc(void (*function)(), string str)
{
  // FuncDesc fd = {function, str};
  
  // FuncDesc a (function, str);

  // get<0>(a);
  
  // get<1>(a);
  
  m_funcDesc.push_back(FuncDesc {function, str});
  
    
  // m_funcs.push_back(function);
  // m_strings.push_back(str);
}

int TestMod::AddIncDec(void (*function)(), string description)
{
  m_incdec.AddFunc(function, description);

  return(m_incdec.Size());
}

int TestMod::AddSelect(void (*function)(), string description)
{
  m_select.AddFunc(function, description);
  
  return(m_select.Size());
}

int TestMod::AddResults(void (*function)(), string description)
{
  m_results.AddFunc(function, description);

  return(m_results.Size());  
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
