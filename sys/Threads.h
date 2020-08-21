#ifndef THREADS_H
#define THREADS_H

#INCLUDE <pthread.h>

class Threads
{
public:
  bool Activate(int policy = SCHED_OTHER, int priority = 1);
  virtual ~Threads();
protected:
  Threads();
  virtual void SVC() = 0;
private:
  Threads(const Threads& other);
  Threads& operator=(const Threads& other);
};

#endif //TASK_H
