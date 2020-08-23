#include <iostream>
#include "Threads.h"

using namespace std;

Threads::Threads()
{
}

Threads::~Threads()
{
}

void* Threads::ThreadRun(void* This)
{
  Threads* pThread = (Threads *)This;

  pThread->Run();

  return(NULL);
}

bool Threads::Activate(int policy, int priority)
{
  bool status = false;

  int code = 0;
  pthread_t threadStruct;
  pthread_attr_t threadAttributes;
  struct sched_param threadSchedParameters;
  threadSchedParameters.sched_priority = priority;

  // DEBUG cout << policy << " : " << priority << " : " << sched_get_priority_max(policy) << " : " << sched_get_priority_max(policy) << std::endl;
  
  // Get attribute struct for setting priority and policy
  if ((code = pthread_attr_init(&threadAttributes)) != 0)
  {
    cout << "FAILED: pthread_attr_init" << std::endl;
    status = false;
  }
  // Set the scheduling policy in the attribute structure
  else if ((code = pthread_attr_setschedpolicy(&threadAttributes, policy)) != 0)
  {
    cout << "FAILED: pthread_attr_setschedpolicy" << std::endl;
    status = false;
  }
  // Set the priority in the attribute structure
  else if ( (code = pthread_attr_setschedparam(&threadAttributes, &threadSchedParameters)) != 0)
  {
    cout << "FAILED: pthread_attr_setschedparam code=" << code << " priority=" << priority << std::endl;
    status = false;
  }
  // Create the thread, which starts the Runner function defined by the derived class
  else if ((code = pthread_create(&threadStruct, &threadAttributes, ThreadRun, (void*)this)) != 0 )
  {
    cout << "FAILED: pthread_create code=" << code << std::endl;
    status = false;
  }
  else if ((code = pthread_attr_destroy(&threadAttributes)) != 0)
  {
    cout << "FAILED: pthread_attr_destroy" << std::endl;
    status = false;
  }
  else
  {
    status = true;
  }

  return status;
}
