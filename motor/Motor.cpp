#include <iostream>
#include <pigpio.h>
#include "Motor.h"

using namespace std;

/*------------------------------------------------------------------------------
FUNCTION:      Motor::Motor()
PURPOSE:       
------------------------------------------------------------------------------*/
Motor::Motor()
{
  cout << "Motor::Motor()" <<std::endl;
}

Motor::~Motor()
{
  cout << "Motor::~Motor()" <<std::endl;
}

/*------------------------------------------------------------------------------
FUNCTION:  Motor::Run(void)
PURPOSE:   Run the motor is a separate thread

Step Frequency                Max: 250kHz
Step Pulse duration STEP High Min: 1.9μs, no max
Step Pulse duration STEP Low  Min: 1.9μs, no max

    1.9us 1.9us
   v     v     v
   ,-----.     ,-----.
   |     |     |     |
---'     `-----'     `--
------------------------------------------------------------------------------*/
int Motor::Run(void)
{
  cout << "Motor:Run() in a separate thread" << std::endl;
}
