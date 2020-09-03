# Robot

# Migrate to GitHub for Rick

## ToDo
- Use calibrated data for calls to MPU6050
  - Set a flag when calibration has completed so all function that use
    calibrated data can check if we are calibrated first
- Clean up all loop counters and timers.
  - Example calibration should be same rate as callers rate in Gyro
- Pull data all at once so that it is all synchronized as spelled out
  in data sheet
- Determine how acc_calibration_value was found in original code
- Migrate to usleep() or gpioDelay(uint32_t micros). Note that both
  may do busy loops for time less than 100us.

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
