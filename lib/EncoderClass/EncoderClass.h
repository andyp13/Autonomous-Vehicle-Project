
#define kSampleRate 10
#define kAverageSize 5

class EncoderClass{
public:
  EncoderClass(){}

  bool begin(int pin);
  void loop();
  long rawHalfSecond();
  float meterPerSecond();

protected:

private:
  long currentTime;
  long lastRunTime;
  long currentCount;
  int arrayCount;
  long averageCount[kAverageSize];
  float conversion;
};
