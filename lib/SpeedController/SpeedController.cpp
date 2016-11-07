/**
* @Author: Aaron Phillips <andyp13>
* @Date:   11-07-2016
* @Email:  andy@phillips101.com
* @Project: Autonomous Vehicle Project
* @Last modified by:   andyp13
* @Last modified time: 11-07-2016
*/

#include<SpeedController.h>

SpeedController::SpeedController(){}

void SpeedController::begin(int newNeutralThrottle){
  neutralThrottle = newNeutralThrottle;
}

int SpeedController::getServoSpeed(float currentSpeed, long wantedSpeed) {
  return (wantedSpeed - currentSpeed) + neutralThrottle;
}
