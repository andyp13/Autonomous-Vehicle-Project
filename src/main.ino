#include <inclusions.h>

/*        CLASSES       */

CompassController compass;
SteeringController steering;
ImuController accelGyro;
WifiAccessPoint myAccessPoint;
CommunicationKey updateKey;
I2CSend RcController;

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
  90,   155,  2000, //R
  109,  90,    2000, //S
  30,  90,   3000, //STOP
   80,  90,       0, //Complete
};

CommandController mainController(gCommands, kNeutralThrottle,kStraightSteering);

void setup() {
  //Will only run once
  Serial.begin(9600); //Really would like to be faster... depends on board.Test
  while(!Serial);
  Serial.println("Booting Up");

  // Pin Modes
/*  pinMode(kSteeringServoPin, OUTPUT);
  pinMode(kThrottlePin, OUTPUT);*/

  /*      Servo setup       */
  /*steeringServo.attach(kSteeringServoPin);
  steeringServo.write(kStraightSteering);
  escServo.attach(kThrottlePin);
  escServo.write(kNeutralThrottle);*/
  pinMode(LED_BUILTIN, OUTPUT);


  //Class setups
  mainButton.setup();
  //compass.setup();
  accelGyro.setup();
  Serial.println("Main Classes Setup, Setting up Wifi");

  myAccessPoint.setup();
  Serial.println("Wifi Setup");
  Serial.println("Steeting up RC controller");
  RcController.setup();
  Serial.println("All classes setup");

Serial.println("Getting Time");
  //Time Setup
  currentTime = millis();

  Serial.println("Getting compass info");
  steering.setNewHeading(compass.getDegreeHeading());

  //Let the compass/gps wake up
  Serial.println("Warming compass and wifi up.");
  delay(750);
  Serial.println("All done.");
  Serial.println("Starting Loops");

  Serial.println("Starting Main Button Loop");
  Scheduler.startLoop(buttonLoop);

  Serial.println("Starting Main Controller Loop");
  Scheduler.startLoop(mainControllerLoop);

  Serial.println("Starting Access Point Loop");
  Scheduler.startLoop(updateWebpage);

  //Serial.println("Starting RC Controller Loop");
  //Scheduler.startLoop(rcControllerLoop);
}

void rcControllerLoop() {
RcController.returnNumbers();
delay(10);       //UPdate at 100 hertz
}

void buttonLoop() {
  mainButton.loop();
  yield();
}

void mainControllerLoop() {
  mainController.loop();
  yield();
}

void updateWebpage() {
  if (!killswitchFlag) {
    myAccessPoint.updateInformation(mainController.getThrottle(), steering.change(), compass.getDegreeHeading(), steering.getWantedHeading(), !killswitchFlag);
  } else {
    myAccessPoint.updateInformation(kNeutralThrottle, kStraightSteering, compass.getDegreeHeading(), steering.getWantedHeading(), !killswitchFlag);
  }
  delay(500);
}

void loop() {
  //Runs in a loop forever
  //Check Time
  currentTime = millis();

  //Class Loops
  //mainButton.loop();
  //compass.loop();
  //mainController.loop();    //TODO: Replace with correct controller here
  //accelGyro.loop();
  myAccessPoint.loop();

  steering.setCurrentHeading(compass.getDegreeHeading());

  //Check  Button Press
  if ( mainButton.didPress() ) {
    delay(kDelayTime);
    mainController.start();      //TODO: Replace with correct controller here
    killswitchFlag = !killswitchFlag;
    steering.setNewHeading(compass.getDegreeHeading());
  }

  //Button If Statements
if (!mainController.isRunning()) {    //If the controller is not running
    killswitchFlag = true;                //make sure nothing is running
  }           //TODO: Replace with correct controller here*/

  //Find where to Turn
  steering.headingChange(mainController.getSteering());    //TODO: Replace with correct controller here

//As long as the button was not pushed. write to the servo
  if(!killswitchFlag) {
  RcController.changeVariables(mainController.getThrottle(), steering.getWantedHeading());
  } else {
    RcController.changeVariables(kNeutralThrottle, kStraightSteering);
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
  /*Serial.print("Acel:");Serial.print("\n");
  Serial.print("x: ");Serial.print(accelGyro.getAccelX()); Serial.print("\t");
  Serial.print("y: ");Serial.print(accelGyro.getAccelY()); Serial.print("\t");
  Serial.print("z: ");Serial.print(accelGyro.getAccelZ()); Serial.print("\n");
  Serial.print("Gyro:"); Serial.print("\n");
  Serial.print("x: ");Serial.print(accelGyro.getGyroX()); Serial.print("\t");
  Serial.print("y: ");Serial.print(accelGyro.getGyroY()); Serial.print("\t");
  Serial.print("z: ");Serial.print(accelGyro.getGyroZ()); Serial.print("\n");*/
  lastSerialTime = currentTime;
}



//Update webpage Info

yield();

}
/** Things to do... no perticular order. Not all need to be done... When you finish it. delete and push.
 *TODO: Redo Controller Class to allow for GPS waypoints
 *TODO: Make a class for UltraSonic Sensors
 *TODO: Maybe: Make a class for LiDar Applications
 *TODO: Add a GPS controller class
 *TODO: Figure out how much RAM this will all use and find a controller board
 *TODO: Add a Accel controller class
 *TODO: Add a gyro Controller Class
 *TODO: Add an EncoderClass
 **/
