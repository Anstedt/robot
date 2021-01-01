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
  void AddFunc(void (*function)(int), string str);
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
    function<void(int)> m_function;
    string m_string;
  } FuncDesc;
  
  vector<FuncDesc> m_funcDesc;
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
  int AddIncDec(void (*function)(int), string description);
  int AddSelect(void (*function)(int), string description);
  int AddResults(void (*function)(int), string description); 
  bool ProcessKeys();

  bool RegisterForCallback(std::function<void(int)> callback, string);
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
  // C++ functional supports callback functions
  vector<std::function<void(int)>> m_callback;
  vector<string> m_description;
  // Static (shared) class variables
};

#endif /* TESTMOD_H */
