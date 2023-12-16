cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
cmake_policy(VERSION 3.18)
project(Robot)

include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/mpu6050")
include_directories("${PROJECT_SOURCE_DIR}/gyro")
include_directories("${PROJECT_SOURCE_DIR}/sys")
include_directories("${PROJECT_SOURCE_DIR}/controller")
include_directories("${PROJECT_SOURCE_DIR}/balancer")
include_directories("${PROJECT_SOURCE_DIR}/motors")
include_directories("${PROJECT_SOURCE_DIR}/servo")
include_directories("${PROJECT_SOURCE_DIR}/legs")
include_directories("${PROJECT_SOURCE_DIR}/test")

message("${PROJECT_SOURCE_DIR}")

add_subdirectory(sys)
add_subdirectory(gyro)
add_subdirectory(mpu6050)
add_subdirectory(controller)
add_subdirectory(balancer)
add_subdirectory(motors)
add_subdirectory(servo)
add_subdirectory(legs)
add_subdirectory(test)

add_executable(robot robot.cpp)

target_link_libraries(robot controller balancer gyro motors mpu6050 legs servo test pigpio threads rt pthread)

# robot needs to run as root
add_custom_command(TARGET robot POST_BUILD
  OUTPUT ${output}
  COMMAND sudo chown root:$ENV{USER} robot
  COMMAND sudo chmod ugoa+s robot
  COMMENT "Hello")

# Additional C Compiler flags
# Note: -Werror causes all warnings to be errors that will halt the build
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -Wall -Wextra -Wno-unused-parameter -Wno-endif-labels -Wno-cpp -Wno-null-dereference -Wno-unused-value -Wno-address -Wno-unused-variable -Wno-unused-but-set-variable -Wno-ignored-qualifiers -Wno-unused-function -Wno-unused-local-typedefs -Wno-maybe-uninitialized -Werror ${IINC} ${IINC2} -D_REENTRANT -fsigned-char -fsigned-bitfields")

# Additional C++ Compiler flags
# Note: -Werror causes all warnings to be errors that will halt the build
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=c++14 -Wall -Wextra -Wno-unused-parameter -Wno-endif-labels -Wno-cpp -Wno-null-dereference -Wno-unused-value -Wno-address -Wno-unused-variable -Wno-unused-but-set-variable -Wno-ignored-qualifiers -Wno-unused-function -Wno-unused-local-typedefs -Wno-maybe-uninitialized -Werror ${IINC} ${IINC2} -D_REENTRANT -fsigned-char -fsigned-bitfields -fno-exceptions")
