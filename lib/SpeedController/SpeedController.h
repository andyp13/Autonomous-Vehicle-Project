/**
* @Author: Aaron Phillips <andyp13>
* @Date:   11-07-2016
* @Email:  andy@phillips101.com
* @Project: Autonomous Vehicle Project
* @Last modified by:   andyp13
* @Last modified time: 11-07-2016
*/

class SpeedController {
public:
  SpeedController();

  void begin(int newNeutralThrottle);
  int getServoSpeed(float currentSpeed, long wantedSpeed);

protected:

private:
  int neutralThrottle;
};
