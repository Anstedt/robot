/*******************************************************************************
PACKAGE:  Robot
FILE:     MPU6050.h

PURPOSE:  Interface to the MPU6050 Gyro/Accel
*******************************************************************************/
#ifndef MPU6050_LIB
#define MPU6050_LIB

class MPU6050
{
public:
  // Local Classes
  // Constructors
  MPU6050();
  // Mutators: non-const operations
  void set_defaults(void);
  void calibrate(void);
  void set_power_mgmt(int value);    // Set power management
  void set_gyro_config(int value);   // GYRO_CONFIG  = 0x00
  void set_accel_config(int value);  // ACCEL_CONFIG = 0x08
  void set_filter_config(int value); // CONFIG       = 0x03 ~43Hz
  // Accessors: const operations
  int get_gyro_X_cal(void); // GYRO_XOUT_H, calibrated
  int get_gyro_Y_cal(void); // GYRO_YOUT_H, calibrated
  int get_gyro_Z_cal(void); // GYRO_ZOUT_H, calibrated
  void get_gyro_XY_cal(int &x, int &y); // Similar to arduino code
  int get_accel_X_cal(void); // ACCEL_XOUT_H, calibrated
  int get_accel_Y_cal(void); // ACCEL_YOUT_H, calibrated
  int get_accel_Z_cal(void); // ACCEL_ZOUT_H Used by Arduino code but may be different for us

  float get_accel_Z_cal_angle(void) { return(m_acc_Z_cal_ang); }; // Experimental
  
  // Static and friend functions
  // Memory management: copy constructor, destructor, operator=
  ~MPU6050();
protected:
private:
  // Local Classes
  // Constructors
  // Mutators: non-const operations
  // Accessors: const operations
  int get_gyro_X(void); // GYRO_XOUT_H
  int get_gyro_Y(void); // GYRO_YOUT_H
  int get_gyro_Z(void); // GYRO_ZOUT_H
  int get_accel_X(void); // ACCEL_XOUT_H
  int get_accel_Y(void); // ACCEL_YOUT_H
  int get_accel_Z(void); // ACCEL_ZOUT_H Used by Arduino code but may be different for us
  int get_accel_cal(int addr);
  int read_raw_data(int addr);
  // Static and friend functions
  // Memory management1
  MPU6050(const MPU6050&);
  MPU6050& operator=(const MPU6050& rhs);
  int device_fd;
  bool m_calibrated;

  // HJA PID code not here?
  float self_balance_pid_setpoint;
  float pid_error_temp, pid_i_mem, pid_setpoint, pid_output, pid_last_d_error;
  float pid_output_left, pid_output_right;

  // Data fields
  int m_acc_calibration_value;
  float m_acc_Z_cal_ang; // Experimental
  
  long m_gyro_yaw_calibration_value;
  long m_gyro_pitch_calibration_value;
  // constants
  static const int Device_Address = 0x68;	/*Device Address/Identifier for MPU6050*/
  static const int SMPLRT_DIV     = 0x19;
  static const int CONFIG         = 0x1A;
  static const int GYRO_CONFIG    = 0x1B;
  static const int ACCEL_CONFIG   = 0x1C;
  static const int INT_ENABLE     = 0x38;
  static const int ACCEL_XOUT_H   = 0x3B;
  static const int ACCEL_YOUT_H   = 0x3D;
  static const int ACCEL_ZOUT_H   = 0x3F;
  static const int GYRO_XOUT_H    = 0x43;
  static const int GYRO_YOUT_H    = 0x45;
  static const int GYRO_ZOUT_H    = 0x47;
  static const int PWR_MGMT_1     = 0x6B;
  // Static (shared) class variables
};
#endif
