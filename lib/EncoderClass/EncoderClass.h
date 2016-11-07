/**
* @Author: Aaron Phillips <andyp13>
* @Date:   11-05-2016
* @Email:  andy@phillips101.com
* @Project: Autonomous Vehicle Project
* @Last modified by:   andyp13
* @Last modified time: 11-07-2016
*/

#include<Arduino.h>

#define kSampleRate 10
#define kAverageSize 5

class EncoderClass{
public:
  EncoderClass();

  bool begin(int pin);
  void loop();
  long rawHalfSecond();
  float meterPerSecond();

protected:

private:
  int pin;
  long currentTime;
  long lastRunTime;
  long currentCount;
  int arrayCount;
  long averageCount[kAverageSize];
  float conversion;
};
