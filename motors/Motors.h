/*******************************************************************************
PACKAGE: Robot
FILE:    Motors.h

PURPOSE: Interface to PICO for motor control 
*******************************************************************************/

/* NOTES ***********************************************************************
g++ Motors.cpp -o Motors -lpigpio
sudo ./Motors
*******************************************************************************/

#ifndef MOTORS_H
#define MOTORS_H

/* INCLUDE ********************************************************************/

/* CLASSES ********************************************************************/
/*------------------------------------------------------------------------------
CLASS:	 Motors

PURPOSE: Motor control
------------------------------------------------------------------------------*/
class Motors
{
public:
  // Local Classes
  // Constructors
  Motors();
  // Mutators: non-const operations
  // Accessors: const operations
  // Static and friend functions
  // Memory management: copy constructor, destructor, operator=
  ~Motors() {};
private:
  // Local Classes
  // Constructors
  // Mutators: non-const operations
  // Accessors: const operations
  // Static and friend functions
  // Memory management
  Motors(const Motors&);
  Motors& operator=(const Motors& rhs);
  // Data fields
  // Static (shared) class variables
};


#endif /* MOTORS_H */
