#include <inclusions.h>

/*        CLASSES       */

CompassController compass;
SteeringController steering;
ImuController accelGyro;

Servo steeringServo;
Servo escServo;
Button mainButton = Button( kMainButtonPin );

/*   GLOBAL VARIABLES   */
//safty
bool killswitchFlag = true;
//Time
int currentTime = 0;
int lastSerialTime = 0;
////Commands
long gCommands[] = {
// Th  ST    FADE
  109, 90,    7650, //S
  90,   155,  2000, //R
  109,  90,    1350, //S
  90,   180,  2000, //R
  109,  90,    2000, //S
  30,  90,   3000, //STOP
   80,  90,       0, //Complete
};

CommandController mainController(gCommands, kNeutralThrottle,kStraightSteering);

void setup() {
  //Will only run once
  Serial.begin(9600); //Really would like to be faster... depends on board.Test

  // Pin Modes
  pinMode(kSteeringServoPin, OUTPUT);
  pinMode(kThrottlePin, OUTPUT);

  /*      Servo setup       */
  steeringServo.attach(kSteeringServoPin);
  steeringServo.write(kStraightSteering);
  escServo.attach(kThrottlePin);
  escServo.write(kNeutralThrottle);


  //Class setups
  mainButton.setup();
  compass.setup();
  accelGyro.setup();

  //Time Setup
  currentTime = millis();

  //Let the compass/gps wake up
  delay(750);
}

void loop() {
  //Runs in a loop forever
  //Check Time
  currentTime = millis();

  //Class Loops
  mainButton.loop();
  compass.loop();
  mainController.loop();    //TODO: Replace with correct controller here
  accelGyro.loop();

  //Check  Button Press
  if ( mainButton.didPress() ) {
    delay(kDelayTime);
    mainController.start();      //TODO: Replace with correct controller here
    killswitchFlag = !killswitchFlag;
    //startHeading = compass.getDegreeHeading();
    //newHeading = startHeading;
  }

  //Button If Statements
if (!mainController.isRunning()) {    //If the controller is not running
    killswitchFlag = true;                //make sure nothing is running
  }           //TODO: Replace with correct controller here*/

  //Find where to Turn
  steering.headingChange(mainController.getSteering());    //TODO: Replace with correct controller here

//As long as the button was not pushed. write to the servo
  if(!killswitchFlag) {
  escServo.write( mainController.getThrottle());    //TODO: Replace with the correct  controller here
  steeringServo.write( steering.change());
  } else {
    escServo.write(kNeutralThrottle);
    steeringServo.write(kStraightSteering);
  }

  //Print Serial Info
  if(currentTime - lastSerialTime > kSerialOutputTime) {
  Serial.print( steering.change() );
  Serial.print("\t");
  Serial.print(compass.getDegreeHeading());
  Serial.print("\t");
  Serial.print(compass.getCount());
  Serial.print("\t");
  Serial.print(steering.getWantedHeading());
  Serial.print("\n" );
  Serial.print("Acel:\t");
  Serial.print("x: ");Serial.print(accelGyro.getAccelX()); Serial.print("/t");
  Serial.print("y: ");Serial.print(accelGyro.getAccelY()); Serial.print("/t");
  Serial.print("z: ");Serial.print(accelGyro.getAccelZ()); Serial.print("/n");
  Serial.print("Gyro: \t");
  Serial.print("x: ");Serial.print(accelGyro.getGyroX()); Serial.print("/t");
  Serial.print("y: ");Serial.print(accelGyro.getGyroY()); Serial.print("/t");
  Serial.print("z: ");Serial.print(accelGyro.getGyroZ()); Serial.print("/t");
  lastSerialTime = currentTime;
}

}
/** Things to do... no perticular order. Not all need to be done... When you finish it. delete and push.
 *TODO: Redo Compasss Class (If needed)
 *TODO: Redo Controller Class to allow for GPS waypoints
 *TODO: Make a class for UltraSonic Sensors
 *TODO: Add a wireless system (btle, 802.11, etc.)
 *TODO: MultiTasking(Only with Due)
 *TODO: Maybe: Make a class for LiDar Applications
 *TODO: Add a GPS controller class
 *TODO: Figure out how much RAM this will all use and find a controller board
 *TODO: Add a Accel controller class
 *TODO: Add a gyro Controller Class
 *TODO: Add an EncoderClass
 **/
