# Robot

### See /mnt/robot/RP/notes.txt for important information on driver
    and implementation

### Transition to NEW motor system
- Options
  - Dual motor objects?
    - Dual Fifos would work since Gyro Callback goes through Balancer?
  - Is there a need for separate threads?
    - I could call direct from Gyro, through Balancer, but that could stall Gyro in some cases.
    - Biggest issue is Fifo's are polled so the Motor threads still need to run at 250Hz each
    - With Rick's driver the overhead of calls should be minimal so direct calls would work
    - Changes to do this
      - Motor threads removed
      - Motor->AddGyroData() is direct call to Rick's driver
      - Ask Rick if a combined call is best?
        - This could be a static motor call used by each Motor???
- Plan
  - Dual motor Objects
  - Phase one is keep threads
  - Phase two is direct calls, not threads, after I learn more about Rick's driver
  - Phase three, optional. static function to combine both motor calls.

### Transition to Linux driver for motor control
- Build and install driver
- Learn how driver works
- Move to 2 Motor objects rather than 1 Motor object controlling 2 motors
  - This means the Balancer needs to create 2 motors.
  - Question, should the Motor objects share the same running thread?
    - Might need this since there is only one Gyro data Fifo
    - But the Gyro data could be sent to 2 Fifos in AddGyroData()
    - See CallBack in Balancer
- Test out driver on my system
- Work out max speeds based on different micro-step setting

### Implement PID based off of Arduino Code
- Add code to mys system and get it to build
- Move from angle based control to speed and distance control

### Implement mechanism to control robot motion; turning and traveling

### Angles

The front is where the holes are in the upper body. The angles are in reference to the front.
 90: is 90 degrees forward of the front
  0: is straight down
-90: is 90 degrees backwards from the front

### Config
- Config.h has Motor and Legs IO and HW configurations.

### NOTES

### System Logging
- #include "Slog.h" in robot.cpp
- slog << "My Notes" << std::endl;
  "My Notes" in /var/log/message
- SLOG << "My Notes" << std::endl;
  "My Notes" in /var/log/message
  "robot.cpp:33 My Notes" in /var/log/message

### Legs

- Right and Left leg joints are now roughly correct for 90(forward), 0(straight) and -90(back) degress.

## Controller
- Creates and controls the robot, constructed from robot.c::main()
- Control interface is still needed.

## Status
- robot
  - Signals
- Controller - just a shell at this time, needs controller interfaces and commands
  - Balancer - needs pid, motion commands, dual motor testing, balance testing
    - Motor - works
    - DRV8825 - works but should be a Linux driver
    - Gyro - works but needs Z averaging on startup
    - MPU6050 - works and tested
  - Legs
    - Leg - works but may need interfaces added
    - Joint - works but may need interfaces added
    - Servo - works but may need interfaces added
      - Interfaces to PCA9685 16 Channel 12 Bit PWM Servo Driver
  - Threads - works and tested

## The robot needs to run as root but cmake takes care of this during the build

## ToDo
- Add a command line interface for setting:
  - Run duration
  - Default motor speed
  - Turning on debug
  - Setting motor mode
- Add a speed interface so balancer can more smoothly control the
  motor. May be able to use this to more smoothly get to the final
  location. Rick is thinking of this as well by having ramping
  functions for startup and shutdown. He is thinking of adding this to
  the driver itself. Might be the right thing to do.
- Pull data all at once so that it is all synchronized as spelled out
  in data sheet

## Data Flow Diagram of the Robot Code
![Robot](Robot.png)

## Electrical Harware Design
![Robot-HW](Robot-HW.png)

## Migrating to c++
- Notice how C++ exceptions are turned off, see CMakeLists.txt
  compiler option CMAKE_CXX_FLAGS. This is to improve performance as
  exceptions can slow the code down drastically and we are trying to
  build a real-time robot control system.

## Option 1: use cmake and Ninja for the build
- Install cmake
  - $ sudo apt-get install cmake
- Install ninja
  - $ sudo apt-get install ninja-build
  - $ sudo ln /usr/bin/ninja /usr/sbin/ninja
- Build the robot using ninja
  - cmake -G Ninja
  - ninja
- Run the robot
  - $ robot

## Option 2: Using cmake and make for the build
- Install cmake
  - $ sudo apt-get install cmake
- Build the robot using make
  - $ cd robot # wherever you have it
  - $ cmake . # you need the '.'
  - $ make
- Run the robot
  - $ ./robot

- To start over with cmake run the following
  - $ rm -rf rules.ninja CMakeCache.txt */CMakeCache.txt CMakeFiles */CMakeFiles -r build.ninja .ninja_* cmake_install.cmake */cmake_install.cmake */lib*.a
