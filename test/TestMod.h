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
#include "keypress.h"

/* CLASSES ********************************************************************/
class Funcs
{
public:
  Funcs() {};
  void AddFunc(void (*function)());
  ~Funcs() {};
private:
  Funcs(const Funcs&) {};
  Funcs& operator=(const Funcs& rhs);
  // Data fields
  std::vector<std::function<void(void)>> m_funcs;
};

/*------------------------------------------------------------------------------
CLASS:   TestMod
PURPOSE:       
------------------------------------------------------------------------------*/
class TestMod
{
public:
  // Local Classes
  // Constructors
  TestMod() {};
  // Mutators: non-const operations
  bool AddIncDec(Funcs array);
  bool AddSelect(Funcs array);
  bool AddResults(Funcs array); 
  bool ProcessKeys();
  // Accessors: const operations
  // Static and friend functions
  // Memory management: copy constructor, destructor, operator=
  ~TestMod() {};
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
  Funcs m_incdec;
  Funcs m_select;
  Funcs m_results;
  // Static (shared) class variables
};

#endif /* TESTMOD_H */
