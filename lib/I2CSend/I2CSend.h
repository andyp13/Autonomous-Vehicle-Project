#include <config.h>

class I2CSend {
public:
  I2CSend(){}

  void setup() {
    Wire.begin();
    }

    void returnNumbers() {
      Wire.beginTransmission(kArduinoSlaveAddress);
      I2C_writeAnything(throttle);
      I2C_writeAnything(wantedHeading);
      Wire.endTransmission();
    }



  void changeVariables(int newThrottle, double newHeading){
        throttle = newThrottle;
        wantedHeading = newHeading;
  }

private:
  int throttle;
  double wantedHeading;
};
