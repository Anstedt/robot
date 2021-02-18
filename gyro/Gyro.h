#ifndef GYRO_H
#define GYRO_H

/* INCLUDE ********************************************************************/
#include "Threads.h"
#include "MPU6050.h"
#include <functional>

using namespace std;

class Gyro : public Threads
{
public:
  // Local Classes
  // Constructors
  Gyro();
  // Mutators: non-const operations
  // C++ method for cleanly handling callbacks in classes
  bool RegisterForCallback(std::function<void(int, int, float, float)> callback);
  // Accessors: const operations
  // Static and friend functions
  // Memory management: copy constructor, destructor, operator=
  ~Gyro();
protected:
   int Run(void); // Thread entry point
private:
  // Local Classes
  MPU6050* p_mpu6050;
  // Constructors
  // Mutators: non-const operations
  // Accessors: const operations
  // Static and friend functions
  // Memory management
  Gyro(const Gyro&);
  Gyro& operator=(const Gyro& rhs);
  // Data fields
  // C++ functional supports callback functions
  std::function<void(int, int, float, float)> m_callback;
  int m_start;
  int m_gyro_pitch_data_raw;
  int m_gyro_yaw_data_raw;
  float m_angle_acc;
  float m_angle_gyro;
  // Static (shared) class variables
};
#endif
