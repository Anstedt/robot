/*******************************************************************************
PACKAGE:  Robot
FILE:     Threads.cpp

PURPOSE:  Linux Thread Support
*******************************************************************************/
#include <iostream>
#include "Threads.h"

using namespace std;

/*------------------------------------------------------------------------------
FUNCTION:  Threads::Threads()
------------------------------------------------------------------------------*/
Threads::Threads()
{
  m_running = false;
  m_stopped = true;
}

/*------------------------------------------------------------------------------
FUNCTION:  Threads::~Threads()
------------------------------------------------------------------------------*/
Threads::~Threads()
{
}

/*------------------------------------------------------------------------------
FUNCTION:  void* Threads::ThreadRun(void* This)
PURPOSE:   Thread Runner

ARGUMENTS: Object this pointer

RETURNS:   None
------------------------------------------------------------------------------*/
void* Threads::ThreadRun(void* This)
{
  Threads* pThread = (Threads *)This;

  cout << "pThread=" << pThread << std::endl;

  pThread->Run();

  return(NULL);
}


/*------------------------------------------------------------------------------
FUNCTION:  bool Threads::Activate(int policy, int priority)
PURPOSE:   Start the thread

ARGUMENTS: Thread policy and riority

RETURNS:   None
------------------------------------------------------------------------------*/
bool Threads::Activate(int policy, int priority)
{
  bool status = false;
  m_running = true;

  int code = 0;
  pthread_attr_t threadAttributes;
  struct sched_param threadSchedParameters;
  threadSchedParameters.sched_priority = priority;

  cout << policy << " : " << priority << " : " << sched_get_priority_max(policy) << " : " << sched_get_priority_max(policy) << std::endl;

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
  else if ((code = pthread_create(&m_threadStruct, &threadAttributes, ThreadRun, (void*)this)) != 0 )
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

/*------------------------------------------------------------------------------
FUNCTION:  bool Threads::JoinThread()

RETURNS:   True: Joined thread
------------------------------------------------------------------------------*/
bool Threads::JoinThread()
{
  return(pthread_join(m_threadStruct, NULL));
}

/*------------------------------------------------------------------------------
FUNCTION:  bool Threads::ThreadRunning()

RETURNS:   True: thread is running
------------------------------------------------------------------------------*/
bool Threads::ThreadRunning()
{
  return(m_running);
}

/*------------------------------------------------------------------------------
FUNCTION:  void Threads::StopThreadRun()

RETURNS:   None
------------------------------------------------------------------------------*/
void Threads::StopThreadRun()
{
  m_running = false;
}

/*------------------------------------------------------------------------------
FUNCTION:  bool Threads::ThreadStopped()

RETURNS:   True: Thread is stopped
------------------------------------------------------------------------------*/
bool Threads::ThreadStopped()
{
  return(m_stopped); // True if thread stopped
}
