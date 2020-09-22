#include <iostream>
#include <pigpio.h>
#include "Balancer.h"
#include "Servo.h"

#include <unistd.h>

int main(int argc, char *argv[])
{
  std::cout << "Hello Robbie" << std::endl;

  Servo * p_servo = new Servo();
  
  Balancer* p_balancer = new Balancer();

  delete p_balancer;

  delete p_servo;
  
  return(0);
}
