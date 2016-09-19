

#include<Arduino.h>
#include<Wire.h>

#define address 0x1E

//////A Lot of this code is from Sparkfun's tutorial code.


class CompassController {

public:
  CompassController(){}

  void setup() {

    Wire.begin();

    //Put the compass into operating mode
    Wire.beginTransmission(address);  //open communication
    Wire.write(0x02); //Select Mode register
    Wire.write(0x00); //Continuous measurement mode (Do not shut off)
    Wire.endTransmission();

    i = 0;

  }

  void loop() {

    currentTime = millis();

    if(currentTime - lastCheckTime > 10) {
      count++;

      //Tell where to get Data
      Wire.beginTransmission(address);
      Wire.write(0x03);
      Wire.endTransmission();
    //Read Data from each axis, 2 registers per axis
    Wire.requestFrom(address, 6);
    if (6 <= Wire.available()) {
      x = Wire.read() << 8; //x Most Significant Bit
      x |= Wire.read(); //X Least Significant Bit
      z = Wire.read() << 8; //z Most Significant Bit
      z |= Wire.read(); //z Least Significant Bit
      y = Wire.read() << 8; //y Most Significant Bit
      y |= Wire.read(); //y Least Significant Bit
    }

    h = atan2(y,x);
    if (h < 0) {  //If heading is negative. make positive
      h += 2*PI;
    }

    degreeHeading = degrees(h); //Convert to degrees

    if(i < 4) {
      average[i] = degreeHeading;
      i++;
    } else {
      i = 0;
      average[i] = degreeHeading;
    }


    lastCheckTime = currentTime;

  }

  }

  double getDegreeHeading() {
    return degreeHeading;
    Serial.println(degreeHeading);
  }

  double getAverageHeading() {
    double sum = 0.0;
    for (int j =0; j < 4; j++) {
      sum += average[i];
    }
    return sum/4;
  }

  int getCount() {
    return count;
  }

private:
  int x;
  int y;
  int z;
  double h;
  double degreeHeading;
  long lastCheckTime;
  long currentTime;
  int i;
  double average[4] = {0};
  int count;
};
