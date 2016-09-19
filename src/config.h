/*            PINS                   */
#define BUTTON_PIN 7
#define STEERING_PIN 5
#define THROTTLE_PIN 6

/*          DEFAULT VALUES           */
#define kNuteralThrottle 80
#define kStraightSteering 91

/*          CONVERSIONS              */
#define kFeetToSecondConv 20

/*          CompassController        */
#define address 0x1E              //I2C address

/*            TIMINGS                */
#define kDelayTime 1000
#define kSerialOutputTime 250

/*          ERROR CORRECTION         */
#define kMarginOfHeadingError 2
