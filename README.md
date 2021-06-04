# Robot

## You will need pigpio which is part of the default raspberry pi OS install

## BUILDING uses cmake and Ninja for the build
- Install cmake
  - $ sudo apt-get install cmake
- Install ninja
  - $ sudo apt-get install ninja-build
  - $ sudo ln /usr/bin/ninja /usr/sbin/ninja

## Get robot code
- cd to YOUR_GIT_ROOT
- git clone https://github.com/Anstedt/robot.git
- cd robot
- git checkout test-stepper-driver
  - This branch has the test code
  
## Build the robot using ninja  
- cmake -G Ninja
- ninja
## Run the robot
- ./robot

## Rebuild again without adding files
- ninja
## Run the robot
- ./robot

## To start over with cmake run the following; NEVER NEEDS TO BE DONE UNLESS YOU MODIFY THE DIRECTORY STRUCTURE OR ADD NEW FILES
  - rm -rf rules.ninja CMakeCache.txt */CMakeCache.txt CMakeFiles */CMakeFiles -r build.ninja .ninja_* cmake_install.cmake */cmake_install.cmake */lib*.a

## Be very aware that I used robot/motor/rpi4-stepper.h which is slickly modified for my use

## Code that controls the motors
- robot/balancer/Balancer CallBack(...)
  - Data from Gyro is sent to the Motors via this call
    - Notice no delays between motors calls
- robot/motor/Motor.cpp AddGyroData(...)
  - Adds Gyro data to the FIFO
- robot/motor/Motor.cpp Run(void)
  - This pulls the gyro data from the FIFO and calls
    - robot/motor/MotorDriver::MotorCmd(..)
      - Writes to the Linux motor driver

## Call tree
- robot/gyro/Gyro.cpp Run(void) simulates Gyro/accel data and calls m_callback()
  - which is robot/balancer/Balancer CallBack(...) which calls
    - robot/motor/Motor.cpp AddGyroData() which queues up the data in FIFO
      - robot/motor/Motor.cpp Run(void) pulls data from FIFO, m_angle_gyro_fifo
        - robot/motor/Motor.cpp Run(void) sends commands to Stepper Motor Driver

## The FIFO is a blocking queue that will not return unless there is data in it

## The simulation of data is done by removing the MPU6050 code and creating values in the Gyro.cpp code
- Angle values cycle through 0, 2, 4, 0, -2, -4
  - Steps 0, 35, 71, 0, -35, -71
  - In reality each motor runs the opposite directions so if motor A steps == 71, B == -71
- You can hack the simulation in robot/gyro/Gyro.cpp Run(void)
  - Note you can control the rate by changing the following line in the same function
    - timer = gpioTick() + 4000; // gpioTick is in micro seconds
  - This time drives the rate of calls to the motor driver
  - Normally this function is pulling real-time MPU6050 data from the Gyro/Accel hardware
