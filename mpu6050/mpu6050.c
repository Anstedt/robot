/*
	MPU6050 Interfacing with Raspberry Pi
*/

/* gcc -o mpu mpu6050.c -lwiringPi -lm */

#include <wiringPiI2C.h>
#include <stdlib.h>
#include <stdio.h>
#include <wiringPi.h>
#include <math.h>

#define Device_Address 0x68	/*Device Address/Identifier for MPU6050*/

#define SMPLRT_DIV   0x19
#define CONFIG       0x1A
#define GYRO_CONFIG  0x1B
#define ACCEL_CONFIG 0x1C
#define INT_ENABLE   0x38
#define ACCEL_XOUT_H 0x3B
#define ACCEL_YOUT_H 0x3D
#define ACCEL_ZOUT_H 0x3F
#define GYRO_XOUT_H  0x43
#define GYRO_YOUT_H  0x45
#define GYRO_ZOUT_H  0x47
#define PWR_MGMT_1   0x6B

float AccX,AccY,AccZ;
float GyroX,GyroY,GyroZ;

// Arduino
float accAngleX, accAngleY, gyroAngleX, gyroAngleY, gyroAngleZ;
float roll, pitch, yaw;
float AccErrorX, AccErrorY, GyroErrorX, GyroErrorY, GyroErrorZ;
float elapsedTime, currentTime, previousTime;
int c = 0;

float Ax=0, Ay=0, Az=0;
float Gx=0, Gy=0, Gz=0;

int fd;

short read_raw_data(int addr){
	short high_byte,low_byte,value;
  //	high_byte = wiringPiI2CReadReg8(fd, addr);
	// low_byte = wiringPiI2CReadReg8(fd, addr+1);
	// value = (high_byte << 8) | low_byte;
  value = wiringPiI2CReadReg16(fd, addr);
  value = ((value<<8)&0xff00)|((value>>8)&0x00ff);
	return value;
}

void ms_delay(int val){
	int i,j;
	for(i=0;i<=val;i++)
		for(j=0;j<1200;j++);
}

void calculate_IMU_error() {
  // We can call this funtion in the setup section to calculate the accelerometer and gyro data error. From here we will get the error values used in the above equations printed on the Serial Monitor.
  // Note that we should place the IMU flat in order to get the proper values, so that we then can the correct values
  // Read accelerometer values 200 times
  AccErrorX = 0.0;
  AccErrorY = 0.0;
  GyroErrorX = 0.0;
  GyroErrorY = 0.0;
  GyroErrorZ = 0.0;

  while (c < 200) {
		/*Read raw value of Accelerometer and gyroscope from MPU6050*/
		AccX = read_raw_data(ACCEL_XOUT_H) / (16384.0/2); // 4g
    AccY = read_raw_data(ACCEL_YOUT_H) / (16384.0/2); // 4g
    AccZ = read_raw_data(ACCEL_ZOUT_H) / (16384.0/2); // 4g
    // Sum all readings
    AccErrorX = AccErrorX + ((atan((AccY) / sqrt(pow((AccX), 2) + pow((AccZ), 2))) * 180 / M_PI));
    AccErrorY = AccErrorY + ((atan(-1 * (AccX) / sqrt(pow((AccY), 2) + pow((AccZ), 2))) * 180 / M_PI));
    c++;
  }
  //Divide the sum by 200 to get the error value
  AccErrorX = AccErrorX / 200;
  AccErrorY = AccErrorY / 200;
  c = 0;
  // Read gyro values 200 times
  while (c < 200) {
    GyroX = read_raw_data(GYRO_XOUT_H);
    GyroY = read_raw_data(GYRO_YOUT_H);
    GyroZ = read_raw_data(GYRO_ZOUT_H);

    // Sum all readings
    GyroErrorX = GyroErrorX + (GyroX / 131.0);
    GyroErrorY = GyroErrorY + (GyroY / 131.0);
    GyroErrorZ = GyroErrorZ + (GyroZ / 131.0);
    c++;
  }
  //Divide the sum by 200 to get the error value
  GyroErrorX = GyroErrorX / 200;
  GyroErrorY = GyroErrorY / 200;
  GyroErrorZ = GyroErrorZ / 200;
  // Print the error values on the Serial Monitor
  printf("M_PI:%.3f AEX:%.3f AEY:%.3f GyroErrorX:%.3f GyroErrorY:%.3f GyroErrorZ:%.3f\n", M_PI, AccErrorX, AccErrorY,GyroErrorX, GyroErrorY, GyroErrorZ);
}

void MPU6050_Init(){
  // wiringPiI2CWriteReg8 (fd, PWR_MGMT_1, 0x80); // Write to power management register
  // delay(100);
  // wiringPiI2CWriteReg8 (fd, 0x68, 0x07);
  // delay(100);
  // wiringPiI2CWriteReg8 (fd, PWR_MGMT_1, 0x00); // Write to power managementregister

  wiringPiI2CWriteReg8 (fd, PWR_MGMT_1, 0x00);  // Write to power management register

  wiringPiI2CWriteReg8 (fd, GYRO_CONFIG, 0x00); // Write to Gyro Configuration register
  wiringPiI2CWriteReg8 (fd, ACCEL_CONFIG, 0x08);  // Write to Accel

  // wiringPiI2CWriteReg8 (fd, SMPLRT_DIV, 0x07); /* Write to sample rate register */
  wiringPiI2CWriteReg8 (fd, CONFIG, 0x03);    /* Write to Configuration register */
  // wiringPiI2CWriteReg8 (fd, INT_ENABLE, 0x01); /*Write to interrupt enable register */
  calculate_IMU_error();
  delay(20);
  }

int main(){
  fd = wiringPiI2CSetup(Device_Address);   /*Initializes I2C with device Address*/
  MPU6050_Init();                    /* Initializes MPU6050 */
  float yawsv = 0.0;
  yaw = 0.0;
  roll = 0.0;
  pitch = 0.0;
  elapsedTime = 0.0;
  currentTime = millis();
  previousTime = 0.0;

  while(1)
  {
    /*Read raw value of Accelerometer and gyroscope from MPU6050*/
    AccX = read_raw_data(ACCEL_XOUT_H) / (16384.0/2); // 4g
    AccY = read_raw_data(ACCEL_YOUT_H) / (16384.0/2); // 4g
    AccZ = read_raw_data(ACCEL_ZOUT_H) / (16384.0/2); // 4g

    // Calculating Roll and Pitch from the accelerometer data
    accAngleX = (atan(AccY / sqrt(pow(AccX, 2) + pow(AccZ, 2))) * 180 / M_PI) + 5.920; // AccErrorX ~(0.58) See the calculate_IMU_error()custom function for more details
    accAngleY = (atan(-1 * AccX / sqrt(pow(AccY, 2) + pow(AccZ, 2))) * 180 / M_PI) - 1.608; // AccErrorY ~(-1.58)

    // === Read gyroscope data === //
    previousTime = currentTime;        // Previous time is stored before the actual time read
    currentTime = millis();            // Current time actual time read
    elapsedTime = (currentTime - previousTime) / 1000; // Divide by 1000 to get seconds

    GyroX = read_raw_data(GYRO_XOUT_H) / 131.0;
    GyroY = read_raw_data(GYRO_YOUT_H) / 131.0;
    GyroZ = read_raw_data(GYRO_ZOUT_H) / 131.0;

    // Correct the outputs with the calculated error values
    GyroX = GyroX - GyroErrorX; // GyroErrorX ~(-0.56)
    GyroY = GyroY - GyroErrorY; // GyroErrorY ~(2)
    GyroZ = GyroZ - GyroErrorZ; // GyroErrorZ ~ (-0.8)

    // Currently the raw values are in degrees per seconds, deg/s, so we need to multiply by sendonds (s) to get the angle in degrees
    gyroAngleX = gyroAngleX + GyroX * elapsedTime; // deg/s * s = deg
    gyroAngleY = gyroAngleY + GyroY * elapsedTime;
    yawsv = yaw;
    yaw =  yaw + GyroZ * elapsedTime;
    // printf("yawsv=%.3f \tyaw=%.3f \tGyroX=%.3f \telapsed=%.3f\n", yawsv, yaw, GyroX, elapsedTime);

    // Complementary filter - combine acceleromter and gyro angle values
    roll = 0.96 * gyroAngleX + 0.04 * accAngleX;
    pitch = 0.96 * gyroAngleY + 0.04 * accAngleY;

    // printf("AccX=%.3f \tAccY=%.3f \tAccZ=%.3f\n", AccX, AccY, AccZ);
    // printf("GyroX=%.3f \tGyroY=%.3f \tGyroZ=%.3f\n", GyroX, GyroY, GyroZ);
    printf("roll=%.3f \tpitch=%.3f \tyaw=%.3f \telapsed=%.3f\n", roll ,pitch, yaw, elapsedTime);
    // printf("\n Gx=%.3f °/s\tGy=%.3f °/s\tGz=%.3f °/s\tAx=%.3f g\tAy=%.3f g\tAz=%.3f g\n",Gx,Gy,Gz,Ax,Ay,Az);
    delay(100);
  }
	return 0;
}
