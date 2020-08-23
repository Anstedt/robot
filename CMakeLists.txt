# Make sure we have at least this version of cmake
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project (robot)

include_directories("${PROJECT_SOURCE_DIR}/mpu6050}")
include_directories("${PROJECT_SOURCE_DIR}/gyro}")
include_directories("${PROJECT_SOURCE_DIR}/sys}")

add_subdirectory(sys)
add_subdirectory(gyro)
add_subdirectory(mpu6050)

add_executable(robot robot.cpp)

target_link_libraries(robot mpu6050 wiringPi threads gyro pthread)

# Additional C Compiler flags
# Note: -Werror causes all warnings to be errors that will halt the build
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -Wall -Wextra -Wno-unused-parameter -Wno-endif-labels -Wno-cpp -Wno-null-dereference -Wno-unused-value -Wno-address -Wno-unused-variable -Wno-unused-but-set-variable -Wno-ignored-qualifiers -Wno-unused-function -Wno-unused-local-typedefs -Wno-maybe-uninitialized -Werror ${IINC} ${IINC2} -D_REENTRANT -fsigned-char -fsigned-bitfields")

# Additional C++ Compiler flags
# Note: -Werror causes all warnings to be errors that will halt the build
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=c++14 -Wall -Wextra -Wno-unused-parameter -Wno-endif-labels -Wno-cpp -Wno-null-dereference -Wno-unused-value -Wno-address -Wno-unused-variable -Wno-unused-but-set-variable -Wno-ignored-qualifiers -Wno-unused-function -Wno-unused-local-typedefs -Wno-maybe-uninitialized -Werror ${IINC} ${IINC2} -D_REENTRANT -fsigned-char -fsigned-bitfields -fno-exceptions")