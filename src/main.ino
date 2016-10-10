#define USE_WIFI101_SHIELD


#include <inclusions.h>
#include <config.h>
#include <SPI.h>
#include <WiFi101.h>
#include <Scheduler.h>
#include "ThingSpeak.h"
unsigned long channelNumber   = 169571;
const char * myWriteAPIKey = "YEY98XE2AZO67T1S";


/*        CLASSES       */

float wantedHeading = 0;
float currentHeading = 0;
int currentSteering = 0;
int throttle = 0;


CompassController compass;
SteeringController steering;
ImuController accelGyro;
CommunicationKey updateKey;
I2CSend RcController;

Servo steeringServo;
Servo escServo;
Button mainButton = Button( kMainButtonPin );

/*   GLOBAL VARIABLES   */
//safety
bool killswitchFlag = true;
//Time
int currentTime = 0;
int lastSerialTime = 0;
////Commands
long gCommands[] = {
// Th  ST
  109, 90,    7650, //S
  90,   155,  2000, //R
  109,  90,    1350, //S
  90,   155,  2000, //R
  109,  90,    2000, //S
  30,  90,   3000, //STOP
   80,  90,       0, //Complete
};

CommandController mainController(gCommands, kNeutralThrottle,kStraightSteering);
WiFiClient sslClient;

void setup() {
  //Will only run once
  Serial.begin(9600);
  Wire.begin(kArduinoMasterAddress);
  while(!Serial);
  Serial.println("Booting Up");

  pinMode(LED_BUILTIN, OUTPUT);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // unsuccessful, retry in 4 seconds
    pinMode(LED_BUILTIN, HIGH);
    Serial.print("failed ... ");
    delay(4000);
    Serial.print("retrying ... ");
    pinMode(LED_BUILTIN, LOW);
  }

  Serial.println("Wifi Connected");
  ThingSpeak.begin(sslClient);


  //Class setups
  mainButton.setup();
  //compass.setup();
  accelGyro.setup();
  Serial.println("Main Classes Setup, Setting up Wifi");

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

  Serial.println("Starting RC Controller Loop");
  Scheduler.startLoop(rcControllerLoop);

  Serial.println("Starting Update Numbers");
  Scheduler.startLoop(updateVariables);
}

void updateVariables() {
  wantedHeading = steering.getWantedHeading();
  currentHeading = compass.getDegreeHeading();
  currentSteering = steering.change();
  throttle = mainController.getThrottle();
  yield();
}

void rcControllerLoop() {
  Serial.println(2);
RcController.returnNumbers();

yield();       //UPdate at 100 hertz
}

void buttonLoop() {
  Serial.println(3);
  mainButton.loop();

  yield();
}

void mainControllerLoop() {
  Serial.println(4);
  mainController.loop();

  yield();
}

void updateWebpage() {

  Serial.println(5);
   //Need new stuff
   Serial.println(5.1);
   ThingSpeak.writeField(channelNumber,1, currentHeading,myWriteAPIKey);
   Serial.println(5.2);
   ThingSpeak.writeField(channelNumber,2, wantedHeading,myWriteAPIKey);
   Serial.println(5.3);
   ThingSpeak.writeField(channelNumber,4, currentSteering,myWriteAPIKey);
   Serial.println(5.5);
   ThingSpeak.writeField(channelNumber,3, throttle,myWriteAPIKey);
   Serial.println(5.6);
   delay(1000);
}

void loop() {
  //Runs in a loop forever
  //Check Time
  currentTime = millis();

  steering.setCurrentHeading(compass.getDegreeHeading());

  //Check  Button Press
  if ( mainButton.didPress() ) {
    delay(kDelayTime);
    mainController.start();      //TODO: Replace with correct controller here
    killswitchFlag = !killswitchFlag;
    steering.setNewHeading(currentHeading);
  }

  //Button If Statements
if (!mainController.isRunning()) {    //If the controller is not running
    killswitchFlag = true;                //make sure nothing is running
  }

  //Find where to Turn
  steering.headingChange(mainController.getSteering());

//As long as the button was not pushed. write to the servo
  if(!killswitchFlag) {
  RcController.changeVariables(throttle, wantedHeading);
  } else {
    RcController.changeVariables(kNeutralThrottle, wantedHeading);
  }

  //Print Serial Info
  if(currentTime - lastSerialTime > kSerialOutputTime) {
    //ALL OUTPUTS
  Serial.print( currentSteering );
  Serial.print("\t");

  Serial.print(currentHeading);
  Serial.print("\t");

  Serial.print(compass.getCount());
  Serial.print("\t");

  Serial.print(wantedHeading);
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




yield();

}
