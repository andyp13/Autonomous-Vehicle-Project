#include "AdafruitCompassController.h"

AdafruitCompassController::AdafruitCompassController() {

}

void AdafruitCompassController::displaySensorDetails() {
  sensor_t sensor;
  compass.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" uT");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" uT");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" uT");
  Serial.println("------------------------------------");
  Serial.println("");
}

void AdafruitCompassController::begin(int uniqueName) {
  compass = Adafruit_HMC5883_Unified(uniqueName);

  if(!compass.begin())
  {
    /* There was a problem detecting the HMC5883 ... check your connections */
    Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
    while(1);
  }

  displaySensorDetails();
  count = 0;
}

void AdafruitCompassController::loop() {
  /* Get a new sensor event */
  sensors_event_t event;
  compass.getEvent(&event);

  /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
  Serial.print("X: "); Serial.print(event.magnetic.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.print("  ");Serial.println("uT");

  // Hold the module so that Z is pointing 'up' and you can measure the heading with x&y
  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  float radianHeading = atan2(event.magnetic.y, event.magnetic.x);

  // Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
  // Find yours here: http://www.magnetic-declination.com/
  // Mine is: -13* 2' W, which is ~13 Degrees, or (which we need) 0.22 radians
  // If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
  float declinationAngle = 0.22;
  radianHeading += declinationAngle;

  // Correct for when signs are reversed.
  if(radianHeading < 0)
    radianHeading += 2*PI;

  // Check for wrap due to addition of declination.
  if(radianHeading > 2*PI)
    radianHeading -= 2*PI;
    count++;

}

float AdafruitCompassController::getRadianHeading() {
  return radianHeading;
}

float AdafruitCompassController::getDegreeHeading() {
  return degrees(radianHeading);
}
int AdafruitCompassController::getCount() {
  return count;
}
