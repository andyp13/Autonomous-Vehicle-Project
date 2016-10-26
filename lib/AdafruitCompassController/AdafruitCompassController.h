#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

class AdafruitCompassController {
public:
  AdafruitCompassController();
  void displaySensorDetails();
  void begin(int uniqueName);
  void loop();
  float getDegreeHeading();
  float getRadianHeading();
  int getCount();

protected:

private:
  Adafruit_HMC5883_Unified compass;
  float radianHeading;
  float degreeHeading;
  int count;
};
