# Robot

# PID
PID is running and checked in on branch pid-dev. It reacts very slowly but
does work in both directions.  Seems like the range of my output +/- 10000 vs
the input range +/-180 may be part of this issues.

# Driver Control
Increasing distance causes the driver to take longer on the write calls but also
uses more CPU time. distance=100 %CPU=8 distance=500 %CPU=20

# Ranges
Speed(Pulses per second)(Assume mode 32)
Notes indicate = 3124, which is about 30 RPM
But this is for constant speed, we need maximum when we are a long way off.
Max = 25000 based on my testing, see email, = 235 RPM
So lets try a maximum of 10000, 94 RPM

## Maximum motor pulses per second == 10000
### 94 RPM ~= 1 MPH assuming wheels are 1 ft in diameter

### Using user space app driver for testing
- Swap out real driver by renaming
  - motor/MotorDriver.h -> motor/MotorDriver.h.REAL
  - motor/MotorDriver.cpp -> motor/MotorDriver.cpp.REAL
  - motor/Motor.h -> motor/Motor.h.REAL
  - motor/Motor.cpp -> motor/Motor.cpp.REAL
- Swap in app by renaming
  - motor/MotorDriverApp.h -> motor/MotorDriverApp.h
  - motor/MotorDriverApp.cpp -> motor/MotorDriver.cpp
  - motor/MotorApp.h -> motor/Motor.h
  - motor/MotorApp.cpp -> motor/Motor.cpp
- Rebuild Robot with App driver
- The App mimics the kernel driver interface but is a user space application

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
