# Robot

# Migrating to c++

# Using cmake for the build
- Install cmake
$ sudo apt-get install cmake
- buid robot
$ cd robot # wherever you have it
$ cmake . # you need the .
$ make
$ robot

- To start over with cmake run the following
$ rm -rf rules.ninja CMakeCache.txt */CMakeCache.txt CMakeFiles */CMakeFiles -r build.ninja .ninja_* cmake_install.cmake */cmake_install.cmake

# MPU6050 for balancing

- Works better now but has 2 issues:
  1. Very slowly drifts
  2. Uses current potions as zero when started

# DRV8825 for motor control
