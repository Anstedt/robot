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
class Funcs
{
public:
  Funcs() {};
  void AddFunc(void (*function)(), string str);
  int Size()
  {
    return(m_funcDesc.size());
  };
  ~Funcs() {};
private:
  Funcs(const Funcs&) {};
  Funcs& operator=(const Funcs& rhs);
  // Data fields
  typedef struct FuncDesc
  {
    function<void(void)> m_function;
    string m_string;
  } FuncDesc;
  
  vector<FuncDesc> m_funcDesc;
  // vector<function<void(void)>> m_funcs;
  // vector<string> m_strings;
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
  int AddIncDec(void (*function)(), string description);
  int AddSelect(void (*function)(), string description);
  int AddResults(void (*function)(), string description); 
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
