#include <Wire.h>

class I2CSend {
public:
  I2CSend(){}

  void setup() {
      }

    void returnNumbers() {
      //char buffer[30];
      //Serial.println("converting to string");
      //dtostrf(wantedHeading, 8,4,buffer);
      //Serial.println("Done");

      //Serial.print(throttle);      Serial.print("\t");
      Wire.write(throttle);
      Wire.write((wantedHeading));
    }



  void changeVariables(int8_t newThrottle, int8_t newHeading){
        throttle = newThrottle;
        wantedHeading = newHeading;
  }

private:
  int8_t throttle;
  int8_t wantedHeading;
};
