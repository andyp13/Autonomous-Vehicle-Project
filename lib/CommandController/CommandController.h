
#define kMaxNumCommands 256

// command format: throttle, steering, duration
// zero duration marks end of list
// throttle and steering are assumed to be Servo parameters: (0 to 180, 90 is zero point)

#define kNumberInCommands 3

#include <Arduino.h>

class CommandController
{
public:
  CommandController( long *commands, int throttle_zero = 90, int steering_zero = 90 )
  : _commands( commands ),
  _throttle_zero( throttle_zero ), _steering_zero( steering_zero ),
  _current_command( -1 )
  {

  }

  void setup()
  {

  }

  void loop()
  {
    // Check if stopped
    if ( _current_command < 0 ) return;

    // Check if we have reached the end of the command list
    if ( getDuration() == 0 ) {
      _current_command = -1;
      return;
    }

    // Check duration of current command
    _current_time = millis();
    if ( _current_time - _last_command_time >= getDuration() )
    {
      // Go to next command
      _current_command++;
      _last_command_time = _current_time;

      // Check if we have past the end of the list
      if ( _current_command >= kMaxNumCommands ) _current_command = -1;
    }
  }

  void start()
  {
    _current_command = 0;
    _last_command_time = millis();
  }

  void stop()
  {
    _current_command = -1;
  }

  bool isRunning()
  {
    return ( _current_command >= 0 );
  }

  long getThrottle() {
    if ( _current_command < 0 ) return _throttle_zero;
    return _commands[kNumberInCommands* _current_command+0];
  }

  long getSpecificThrottleFor(int commandWanted) {  //Assuming will recieve valid index number
    if (commandWanted > 0) {
      return _commands[kNumberInCommands*commandWanted+0];
    } else {
      return 0;
    }
  }

  long getSteering() {
    if ( _current_command < 0 ) return _steering_zero;
    return _commands[kNumberInCommands*_current_command+1];
  }

  unsigned long getDuration() {
    if ( _current_command < 0 ) return 0;
    return _commands[kNumberInCommands*_current_command+2]; //3 times because of the three different changes.. +2 last setting
  }

  long getFade() {
    if (_current_command < 0) return 0;
    return _commands[kNumberInCommands*_current_command+3];
  }

  long getThrottleWithFade() {
    if (_current_command < 0) return _throttle_zero;
    long previousThrottle = 0;
    long throttle = getThrottle();
    long fade = getFade();
    long totalFade = 0;
    long returnThrottle = 0;
    long changeThrottle = 0;
    long changeTime = 0;

    if (fade == 0) {
      return getThrottle();
    }

    if(_current_command - 1 > 0) {        //Check there is a command before
      previousThrottle = getSpecificThrottleFor(_current_command-1);
    } else {
      previousThrottle = 0;
    }
      changeThrottle = throttle - previousThrottle;
      if ( (totalFade = (_last_command_time + fade) - _current_time) > 0) {    //The fade is NOT completed
        changeTime = _last_command_time/changeThrottle;

        returnThrottle = previousThrottle + (totalFade/changeTime);
        return returnThrottle;

      } else {                    //Fade is completed
        return getThrottle();
      }

  }

private:
  long *_commands;                  // list of command values (throttle, steering, duration)
  int _throttle_zero;               // zero point for throttle (usually 90 but may be different)
  int _steering_zero;               // zero point for steering (usually 90 but may be different)
  int _current_command;             // current command index (-1 if we haven't started)
  unsigned long _last_command_time; // time (in ms) since last command started
  unsigned long _current_time;      //Current time in ms
};
