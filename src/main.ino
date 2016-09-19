#include "inclusions.h"


/*        CLASSES       */
Button mainButton(kMainButtonPin);
CompassController compass;
SteeringController steering;
Servo steeringServo;
Servo escServo;

/*   GLOBAL VARIABLES   */
//safty
bool killswitchFlag = true;
//heading
double currentHeading;
double newHeading;
//steering
int lastSteeringCommand;
//Time
int currentTime = 0;
int lastSerialTime = 0;

void setup() {
  //Will only run once
  Serial.begin(9600); //Really would like to be faster... depends on board.

  // Pin Modes
  pinMode(kSteeringServoPin, OUTPUT);
  pinMode(kThrottlePin, OUTPUT);

  /*      Servo setup       */
  steeringServo.attach(kSteeringServoPin);
  steeringServo.write(kStraightSteering)
  escServo.attach(kThrottlePin);
  escServo.write(kNuteralThrottle);


  //Class setups
  mainButton.setup();
  compass.setup();

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
  //gController.loop();    TODO: Replace with correct controller here

  //Check  Button Press
  if ( mainButton.didPress() ) {
    delay(kDelayTime);
    //gController.start();      TODO: Replace with correct controller here
    killswitchFlag = !killswitchFlag;
    startHeading = compass.getDegreeHeading();
    newHeading = startHeading;
  }

  //Button If Statements
  if (!killswitchFlag) {    // As long as the killswitch has not been pressed
    /* code */
  } /*else if (!gController.isRunning) {    //If the controller is not running
    killswitchFlag = true;                //make sure nothing is running
  }           TODO: Replace with correct controller here*/

  //Find where to Turn
  steering.headingChange(/*gController.getSteering()*/);    //TODO: Replace with correct controller here

//As long as the button was not pushed. write to the servo
  if(!killswitchFlag) {
  escServo.write( /*gController.getThrottle()*/ );    //TODO: Replace with the correct  controller here
  gSteering.write( steering.change());
  } else {
    gThrottle.write(THROTTLE_ZERO);
    gSteering.write(STEERING_ZERO);
  }

  //Print Serial Info
  if(currentTime - lastSerialTime > kSerialOutputTime) {
  Serial.print( steering.change() );
  Serial.print("\t");
  Serial.print(compass.getDegreeHeading());
  Serial.print("\t");
  Serial.print(compass.getCount());
  Serial.print("\t");
  Serial.print(newHeading);
  Serial.print("\n" );
  lastSerialTime = currentTime;
}

}
/** Things to do... no perticular order. Not all need to be done... When you finish it. delete and push.
 *TODO: Redo Compasss Class (If needed)
 *TODO: Redo Controller Class to allow for GPS waypoints
 *TODO: Make a class for UltraSonic Sensors
 *TODO: Add the button Class
 *TODO: Add a wireless system (btle, 802.11, etc.)
 *TODO: MultiTasking(Only with Due)
 *TODO: Maybe: Make a class for LiDar Applications
 *TODO: Add a GPS controller class
 *TODO: Figure out how much RAM this will all use and find a controller board
 *TODO: Add a Accel controller class
 *TODO: Add a gyro Controller Class
 *TODO: Add an EncoderClass
 **/
