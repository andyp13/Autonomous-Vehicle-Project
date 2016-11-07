#include <inclusions.h>
#include <config.h>
#include <SPI.h>
#include <WiFi101.h>
#include <Scheduler.h>
#include <Wire.h>


/*        CLASSES       */

float wantedHeading = 0;
float currentHeading = 0;
int currentSteering = 0;
int throttle = 0;


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
        130, 90,    4000,//S
        130,   180,  2000,//R
        130,  90,    2000,  //S
        130,   180,  2000,  //R
        130,  90,    2000, //S
        130,   180,  2000,  //R
        130,  90,    2000, //S
        130,   180,  2000,  //R
        30,  90,   3000,   //STOP
        80,  90,       0,  //Complete
};

CommandController mainController(gCommands, kNeutralThrottle,kStraightSteering);
WiFiClient sslClient;

void setup() {
        //Will only run once
        Serial.begin(9600);
        Wire.begin(0x3F);
        //while(!Serial) ;
        Serial.println("Booting Up");
        Wire.onRequest(onRequest);


        pinMode(LED_BUILTIN, OUTPUT);

        //Class setups
        Serial.println("Setting Up main Button");
        mainButton.setup();
        Serial.println("Setting up Accelerometer and Gyro");
        Serial.println("Main Classes Setup, Setting up Wifi");

        Serial.println("Wifi Setup");
        Serial.println("Steeting up RC controller");
        RcController.setup();
        Serial.println("All classes setup");

        Serial.println("Getting Time");
        //Time Setup
        currentTime = millis();



        //Let the /gps wake up
        Serial.println("Warming  and wifi up.");
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
        //Scheduler.startLoop(rcControllerLoop);

        Serial.println("Starting Variable Updater");
        Scheduler.startLoop(updateVariables);

        Serial.println("All DONE!");

}


void onRequest() {
  RcController.returnNumbers();
}


void updateVariables() {
  yield();
}

void rcControllerLoop() {
        RcController.returnNumbers();

        delay(10); //UPdate at 100 hertz
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
        //Need new class here

        delay(1000);
}

void loop() {
        //Runs in a loop forever
        //get Time
        currentTime = millis();


        //Check  Button Press
        if ( mainButton.didPress() ) {
                delay(kDelayTime);  //Should add delay to command... not here
                mainController.start();
                killswitchFlag = !killswitchFlag;
        }

        //Button If Statements
        if (!mainController.isRunning()) { //If the controller is not running
                killswitchFlag = true;    //make sure nothing is running
                mainController.stop();
        }

//As long as the button was not pushed. write to the servo
        if(killswitchFlag == false) {
                RcController.changeVariables((unsigned int8_t)mainController.getThrottle(), (int8_t)map(mainController.getSteering(),0,180,-90,90));
        } else {
                RcController.changeVariables((unsigned int8_t)kNeutralThrottle, (int8_t)map(mainController.getSteering(),0,180,-90,90));
        }

        //Print Serial Info
        if(currentTime - lastSerialTime > kSerialOutputTime) {
                //ALL INPUTS
                Serial.print((unsigned int8_t)mainController.getThrottle()); Serial.print("\t");
                Serial.print((unsigned int8_t)map(mainController.getSteering(),0,180,-90,90)); Serial.println("\t");
                lastSerialTime = currentTime;
        }




        yield();

}
