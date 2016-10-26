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
