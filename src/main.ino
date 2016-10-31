#include <inclusions.h>
#include <config.h>
#include <SPI.h>
#include <WiFi101.h>
#include <Scheduler.h>


/*        CLASSES       */

float wantedHeading = 0;
float currentHeading = 0;
int currentSteering = 0;
int throttle = 0;


ImuController accelGyro;
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
        109, 90,    7650,//S
        90,   155,  2000,//R
        109,  90,    1350,  //S
        90,   155,  2000,  //R
        109,  90,    2000, //S
        30,  90,   3000,   //STOP
        80,  90,       0,  //Complete
};

CommandController mainController(gCommands, kNeutralThrottle,kStraightSteering);
WiFiClient sslClient;

void setup() {
        //Will only run once
        Serial.begin(9600);
        Wire.begin(kArduinoMasterAddress);
        while(!Serial) ;
        Serial.println("Booting Up");

        pinMode(LED_BUILTIN, OUTPUT);

        //Class setups
        Serial.println("Setting Up main Button");
        mainButton.setup();
        //compass.setup();
        Serial.println("Setting up Accelerometer and Gyro");
        accelGyro.setup();
        //compass.begin(12345);
        Serial.println("Main Classes Setup, Setting up Wifi");

        Serial.println("Wifi Setup");
        Serial.println("Steeting up RC controller");
        RcController.setup();
        Serial.println("All classes setup");

        Serial.println("Getting Time");
        //Time Setup
        currentTime = millis();

        Serial.println("Getting compass info");

        //Let the compass/gps wake up
        Serial.println("Warming compass and wifi up.");
        delay(750);
        Serial.println("All done.");
        Serial.println("Starting Loops");

        Serial.println("Starting Main Button Loop");
        Scheduler.startLoop(buttonLoop);

        Serial.println("Starting Main Controller Loop");
        Scheduler.startLoop(mainControllerLoop);

        //Serial.println("Starting Access Point Loop");
        //Scheduler.startLoop(updateWebpage);

        Serial.println("Starting RC Controller Loop");
        Scheduler.startLoop(rcControllerLoop);

        Serial.println("Starting Internal VARIABLES");
        Scheduler.startLoop(updateVariables);
}



void updateVariables() {
        throttle = mainController.getThrottle();
        yield();
}

void rcControllerLoop() {
        RcController.returnNumbers();

        delay(1000); //UPdate at 1 hertz
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
        //Need new stuff

        delay(1000);
}

void loop() {
        //Runs in a loop forever
        //Check Time
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
        }

//As long as the button was not pushed. write to the servo
        if(killswitchFlag == false) {
                RcController.changeVariables(throttle, map(mainController.getSteering(),0,180,-80,80));
        } else {
                RcController.changeVariables(kNeutralThrottle, (int32_t)map(mainController.getSteering(),0,180,-80,80));
        }

        //Print Serial Info
        if(currentTime - lastSerialTime > kSerialOutputTime) {
                //ALL OUTPUTS
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
