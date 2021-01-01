/*******************************************************************************
FILE:          TestModLegs.h
PURPOSE:       
*******************************************************************************/

/* NOTES ***********************************************************************
*******************************************************************************/

/* INCLUDE ********************************************************************/
#include "TestMod.h"
#include "Legs.h"

/* CLASSES ********************************************************************/
/*------------------------------------------------------------------------------
CLASS:	       TestModLegs
------------------------------------------------------------------------------*/
class TestModLegs : public TestMod
{
public:
  // Local Classes
  // Constructors
  TestModLegs(Legs* legs);
  // Mutators: non-const operations
  void Set_RL_Hip(int val);
  void Get_RL_Hip(int val);
  void CallBack(int val);
  // Accessors: const operations
  // Static and friend functions
  // Memory management: copy constructor, destructor, operator=
  ~TestModLegs();
private:
  // Local Classes
  // Constructors
  // Mutators: non-const operations
  // Accessors: const operations
  // Static and friend functions
  // Memory management
  TestModLegs(const TestModLegs&);
  TestModLegs& operator=(const TestModLegs& rhs);
  // Data fields
  Legs* m_legs;
  // Static (shared) class variables
};
/* GLOBALS ********************************************************************/
/* FUNCTIONS ******************************************************************/
/* METHODS ********************************************************************/
