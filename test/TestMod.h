/*******************************************************************************
PACKAGE: Test
PURPOSE: Test Interface      
*******************************************************************************/

/* NOTES ***********************************************************************
*******************************************************************************/

#ifndef TESTMOD_H
#define TESTMOD_H

/* INCLUDE ********************************************************************/
#include <vector>
#include <functional>
#include <iostream>
#include <string>
#include "keypress.h"

using namespace std;

/* CLASSES ********************************************************************/

/*------------------------------------------------------------------------------
CLASS:   TestMod
PURPOSE:       
------------------------------------------------------------------------------*/
class TestMod
{
public:
  // Local Classes
  // Constructors
  TestMod();
  // Mutators: non-const operations
  bool AddIncDec(std::function<void(int)> callback, string);
  bool ProcessKeys();
  // Accessors: const operations
  // Static and friend functions
  // Memory management: copy constructor, destructor, operator=
  ~TestMod();
private:
  // Local Classes
  // Constructors
  // Mutators: non-const operations
  // Accessors: const operations
  // Static and friend functions
  // Memory management
  TestMod(const TestMod&);
  TestMod& operator=(const TestMod& rhs);
  // Data fields
  int m_active;
  int m_incdec;
  // C++ functional supports callback functions
  vector<std::function<void(int)>> m_callback;
  vector<string> m_description;
  // Static (shared) class variables
};

#endif /* TESTMOD_H */
