/**
* @Author: Aaron Phillips <andyp13>
* @Date:   11-02-2016
* @Email:  andy@phillips101.com
* @Project: Autonomous Vehicle Project
* @Last modified by:   andyp13
* @Last modified time: 11-07-2016
*/



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
