#ifndef THREADS_H
#define THREADS_H

#include <pthread.h>

class Threads
{
public:
  bool Activate(int policy = SCHED_FIFO, int priority = 1);
  bool JoinThread() { return(pthread_join(m_threadStruct, NULL)); };
  virtual ~Threads();
protected:
  Threads();
private:
  pthread_t m_threadStruct;
  virtual int Run(void) = 0;
  static void* ThreadRun(void* This);
  Threads(const Threads& other);
  Threads& operator=(const Threads& other);
};

#endif //TASK_H
