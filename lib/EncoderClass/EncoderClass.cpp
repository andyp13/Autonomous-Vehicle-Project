/**
* @Author: Aaron Phillips <andyp13>
* @Date:   11-05-2016
* @Email:  andy@phillips101.com
* @Project: Autonomous Vehicle Project
* @Last modified by:   andyp13
* @Last modified time: 11-07-2016
*/



#include "EncoderClass.h"

EncoderClass::EncoderClass() {}

bool EncoderClass::begin(int newPin) {
  pin = newPin;
  pinMode(pin, INPUT);
  currentTime = millis();
  return true;
}

void EncoderClass::loop() {
  currentTime = millis();
  if(currentTime-lastRunTime > kSampleRate) {
    if(arrayCount > kAverageSize) {
      arrayCount = 0;
    }

    averageCount[arrayCount] = currentCount;


    currentCount = 0;
  }
  if(digitalRead(pin)) {
    currentCount++;
  }


}

long EncoderClass::rawHalfSecond() {
  long sum = 0;
  for (int i = 0; i < kAverageSize; i++) {
    sum += averageCount[i];
  }

  return sum;
}

float EncoderClass::meterPerSecond() {
  return (rawHalfSecond() / kAverageSize) * conversion;

}
