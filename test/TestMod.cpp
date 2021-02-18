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
  cout << "s: decrement and set value" << std::endl;
  cout << "d: select next item" << std::endl;
  cout << "a: select previous item item" << std::endl;
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
        std::cout << "Previous m_active=" << m_description[m_active] << " : " << m_active << std::endl;
        break;
      case 'd':
        // Next item, handle wrap
        if (++m_active > (m_callback.size()-1))
          m_active = 0;
        std::cout << "Next m_active=" << m_description[m_active] << " : " << m_active << std::endl;
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

