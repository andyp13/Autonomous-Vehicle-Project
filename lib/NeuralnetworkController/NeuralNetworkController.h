/**
* @Author: Aaron Phillips <andyp13>
* @Date:   11-02-2016
* @Email:  andy@phillips101.com
* @Project: Autonomous Vehicle Project
* @Last modified by:   andyp13
* @Last modified time: 11-07-2016
*/



#include "Arduino.h"
#include "SD.h"
#include "SPI.h"

class NeuralNetworkController {
public:
        NeuralNetworkController();
        void begin(bool trainingSystem,int maxTime,int timesPerSec, int ssPin);
        void loop();
private:
        int isTraining;
        int throttle[];
        int steering[];
        char* usedFilename;
        File root;
};
