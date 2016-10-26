#include "NeuralNetworkController.h"

NeuralNetworkController::NeuralNetworkController() {

}

void NeuralNetworkController::begin(bool trainingSystem,int maxTime, int timesPerSec, int ssPin) {
  steering[maxTime * 60 * timesPerSec] = {0};
  throttle[maxTime * 60 * timesPerSec] = {0};
        SD.begin(ssPin);

        int fileCount = 0;

        String filename = fileCount + "STE.log";
        char newfilename = *filename.c_str();

        while(SD.exists(&newfilename)) {
                fileCount++;
                String filename = fileCount + "STE.log";
                newfilename = *filename.c_str();
        }

        if (trainingSystem == true) {
                isTraining = true;
                usedFilename = &newfilename;
                /**
                * Make a new steering file
                * make a new throttle file
                */
        } else {
                isTraining = false;
                  //use all files for nn
                  int totalSum = 0;
                  int totalCount = 0;
                  String filename = totalCount + "STE.log";
                  char newFilename = *filename.c_str();
                  while (SD.exists(&newFilename)) {
                    /* code */


                    String filename = totalCount + "STE.log";
                    newFilename = *filename.c_str();
                    totalCount++;
                  }
        }
}

void NeuralNetworkController::loop() {
        if (isTraining == true) {
                //Start Training

        } else {
                //Race Mode
        }
}
