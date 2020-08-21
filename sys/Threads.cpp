#include "sys/Threads.h"

using namespace std;

Threads::Threads()
{
  // do nothing
}

Threads::~Threads()
{
}

bool Threads::Activate(int policy, int priority)
{
  bool status = false;

  int code = 0;
  pthread_t threadStruct;
  pthread_attr_t threadAttributes;
  struct sched_param threadSchedParameters;
  threadSchedParameters.sched_priority = priority;

  // If the attempt to initialize thread attributes failed...
  if ((code = pthread_attr_init(&threadAttributes)) != 0)
  {
    cout << "FAILED: pthread_attr_init" << std:endl
    status = false;
  }
  // Otherwise if the attempt to set the scheduling policy failed...
  else if ((code = pthread_attr_setschedpolicy(&threadAttributes, policy)) != 0)
  {
    cout << "FAILED: pthread_attr_setschedpolicy" << std:endl
    status = false;
  }
  // Otherwise if the attempt to set the priority failed...
  else if ( (code = pthread_attr_setschedparam(&threadAttributes, &threadSchedParameters)) != 0)
  {
    cout << "FAILED: pthread_attr_setschedparam" << std:endl
    status = false;
  }
  else if ((code = pthread_attr_setinheritsched(&threadAttributes, PTHREAD_EXPLICIT_SCHED)) != 0)
  {
    cout << "FAILED: pthread_attr_setinheritsched" << std:endl
    status = false;
  }
  else if ((code = pthread_create(&threadStruct, &threadAttributes, SVC, NULL)) != 0 )
  {
    cout << "FAILED: pthread_create" << std:endl
    status = false;
  }
  else if ((code = pthread_attr_destroy(&threadAttributes)) != 0)
  {
    cout << "FAILED: pthread_attr_destroy" << std:endl
    status = false;
  }
  else
  {
    status = true;
  }

  return status;
}
