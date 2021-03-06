/**
* @Author: Aaron Phillips <andyp13>
* @Date:   11-02-2016
* @Email:  andy@phillips101.com
* @Project: Autonomous Vehicle Project
* @Last modified by:   andyp13
* @Last modified time: 11-07-2016
*/



#include "config.h"
#include "MPU6050.h"

class ImuController{
public:
  ImuController(){}

  void setup() {

//Init
    accelGyro.initialize();



  }

  void loop() {
    accelGyro.getMotion6(&accelX, &accelY, &accelZ, &gyroX, &gyroY, &gyroZ);
  }

  int16_t getAccelX() {
    return accelX;
  }
  int16_t getAccelY() {
    return accelY;
  }
  int16_t getAccelZ() {
    return accelZ;
  }
  int16_t getGyroX() {
    return gyroX;
  }
  int16_t getGyroY() {
    return gyroY;
  }
  int16_t getGyroZ() {
    return gyroZ;
  }


private:

  MPU6050 accelGyro;
  int16_t accelX, accelY, accelZ;
  int16_t gyroX, gyroY, gyroZ;
};
