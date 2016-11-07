/**
* @Author: Aaron Phillips <andyp13>
* @Date:   11-02-2016
* @Email:  andy@phillips101.com
* @Project: Autonomous Vehicle Project
* @Last modified by:   andyp13
* @Last modified time: 11-07-2016
*/



/*            PINS                   */
#define kMainButtonPin 0
#define kSteeringServoPin 5
#define kThrottlePin 6

/*          DEFAULT VALUES           */
#define kNeutralThrottle 79
#define kStraightSteering 90

/*          CONVERSIONS              */
#define kFeetToSecondConv 20

/*          CompassController        */
#define kCompassAddress 0x1E              //I2C compass address

/*          Manager Arduino         */
#define kArduinoSlaveAddress 1
#define kArduinoMasterAddress 2

/*            TIMINGS                */
#define kDelayTime 500
#define kSerialOutputTime 250

/*          ERROR CORRECTION         */
#define kMarginOfHeadingError 2
//Wifi
#define ssid "Andy's iPhone"
#define pass "3032323912"
//Cloud
