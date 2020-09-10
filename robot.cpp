#include <iostream>
#include <pigpio.h>
#include "Balancer.h"

#include <unistd.h>

int main(int argc, char *argv[])
{
  std::cout << "Hello Robbie" << std::endl;

  Balancer* p_balancer = new Balancer();

  delete p_balancer;
  
  return(0);
}
