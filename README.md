# Robot

## Basic Servo interface working for a PCA9685 16 Channel 12 Bit PWM Servo Driver

## Callbacks implemented so listener can get Gyro data as soon as it is produced

## ToDo
- Add a command line interface for setting:
  - Run duration
  - Default motor speed
  - Turning on debug
  - Setting motor mode
- Before using accel Z get an average. Maybe in the Calibrate function
  - While running do not allow large jumps in accel Z to get into the
    current location. Vibrations cause serious gyrations in accel Z.
- Add a speed interface so balancer can more smoothly control the
  motor. May be able to use this to more smoothly get to the final
  location. Rick is thinking of this as well by having ramping
  functions for startup and shutdown. He is thinking of adding this to
  the driver itself. Might be the right thing to do.

- Pull data all at once so that it is all synchronized as spelled out
  in data sheet
- Determine how acc_calibration_value was found in original code

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
  - $ robot

- To start over with cmake run the following
$ rm -rf rules.ninja CMakeCache.txt */CMakeCache.txt CMakeFiles */CMakeFiles -r build.ninja .ninja_* cmake_install.cmake */cmake_install.cmake */lib*.a

## MPU6050 for balancing

- Works better now but has 2 issues:
  1. Very slowly drifts
  2. Uses current potions as zero when started

## DRV8825 for motor control
