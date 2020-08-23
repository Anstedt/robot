#ifndef THREADS_H
#define THREADS_H

#include <pthread.h>

class Threads
{
public:
  bool Activate(int policy = SCHED_FIFO, int priority = 1);
  virtual int Run(void) = 0;
  static void* ThreadRun(void* This);
  virtual ~Threads();
protected:
  Threads();
private:
  Threads(const Threads& other);
  Threads& operator=(const Threads& other);
};

#endif //TASK_H