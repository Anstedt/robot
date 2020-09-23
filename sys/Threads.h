#ifndef THREADS_H
#define THREADS_H

#include <pthread.h>

class Threads
{
public:
  bool Activate(int policy = SCHED_FIFO, int priority = 1);
  bool JoinThread();
  bool ThreadRunning();
  void StopThreadRun();
  bool ThreadStopped();
  bool ThreadReturn()
    {
      return(m_stopped = true);
    }
  virtual ~Threads();
protected:
  Threads();
private:
  bool m_running;
  bool m_stopped;
  pthread_t m_threadStruct;
  virtual int Run(void) = 0;
  static void* ThreadRun(void* This);
  Threads(const Threads& other);
  Threads& operator=(const Threads& other);
};

#endif //TASK_H
