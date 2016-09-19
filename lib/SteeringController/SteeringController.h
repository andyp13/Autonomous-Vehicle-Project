/*TODO:Look at this code and see if it needs optimising */
#include <config.h>

class SteeringController {
public:
  SteeringController(){}

  void headingChange(long steeringCommand) {

    if (lastSteeringCommand != steeringCommand) {   //If the last command is not the current
      if (steeringCommand < kSteeringStraight) {    //Check if it is left
        wantedHeading -= (kSteeringStraight-steeringCommand); //If less subtract the wanted heading by degree number
      } else if (steeringCommand > kSteeringStraight) {   //Check if right
        wantedHeading += (steeringCommand-kSteeringStraight); //If more add the wanted heading by degree number
      }
      lastSteeringCommand = steeringCommand;  //Make sure not to run this a thousand times
      checkHeading();                         //Make sure new heading is an okay number
    }


  }


  int steeringChange() {
    /**
    1. check wantedHeading is different that currentHeading
    2. if wantedHeading is different then figure our whether left or right.
    3. change the steering servo.
    */

    if (wantedHeading != currentHeading) { // if the headings are not the same
      if ((currentHeading- wantedHeading) > 180) {   //If the number is greater than 180 degrees then it prolly goes past north.
        return abs(wantedHeading-currentHeading)+ kSteeringStraight; //go->right
      } else {
      if (wantedHeading < currentHeading) {    //If the wanted heading is less than the current heading
        //Turn Left
        return kSteeringStraight - abs(wantedHeading-currentHeading); //gives number less than 90 degrees
      } else {
        //Turn Right
        return ((wantedHeading-currentHeading))  + kSteeringStraight;  //Gives number greater than 90 degrees

      }
    }

    } else {
       return kSteeringStraight;    //Returns whatever is straignt, as defined int he config
    }

  }


private:
long lastSteeringCommand;
double wantedHeading;
double currentHeading;


    void checkHeading() {  //If the wanted heading is greater than 360 or less than 180 it converts it.
      if (wantedHeading < 0) {
        wantedHeading += 360;
      } if (wantedHeading>359) {
        wantedHeading -= 360;
      }
    }
}
