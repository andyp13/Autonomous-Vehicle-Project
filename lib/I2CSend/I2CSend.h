#include <Wire.h>

class I2CSend {
public:
  I2CSend(){}

  void setup() {
    Wire.begin();
    }

    void returnNumbers() {
      //char buffer[30];
      //Serial.println("converting to string");
      //dtostrf(wantedHeading, 8,4,buffer);
      //Serial.println("Done");

      Wire.beginTransmission(kArduinoSlaveAddress);
      //Serial.println(throttle);
      Wire.write(throttle);
      //Serial.println((int)wantedHeading);
      Wire.write((int)wantedHeading);
      Wire.endTransmission();
    }



  void changeVariables(int newThrottle, float newHeading){
        throttle = newThrottle;
        wantedHeading = newHeading;
  }

private:
  int throttle;
  float wantedHeading;
};
