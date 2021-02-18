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
  void Set_RL_Hip(int val);  // { m_right.Set_Hip(val); };
  void Set_RL_Knee(int val); // { m_right.Set_Knee(val); };
  void Set_LL_Hip(int val);  // { m_left.Set_Hip(val); };
  void Set_LL_Knee(int val); // { m_left.Set_Knee(val); };
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
  // Track pwm settings
  int m_rl_hip;
  int m_rl_knee;
  int m_ll_hip;
  int m_ll_knee;

  // Static (shared) class variables
};
/* GLOBALS ********************************************************************/
/* FUNCTIONS ******************************************************************/
/* METHODS ********************************************************************/
