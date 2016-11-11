
/*
 Debounce

 Each time the input pin goes from LOW to HIGH (e.g. because of a push-button
 press), the output pin is toggled from LOW to HIGH or HIGH to LOW.  There's
 a minimum delay between toggles to debounce the circuit (i.e. to ignore
 noise).

 The circuit:
 * LED attached from pin 13 to ground
 * pushbutton attached from pin 2 to +5V
 * 10K resistor attached from pin 2 to ground

 * Note: On most Arduino boards, there is already an LED on the board
 connected to pin 13, so you don't need any extra components for this example.


 created 21 November 2006
 by David A. Mellis
 modified 30 Aug 2011
 by Limor Fried
 modified 28 Dec 2012
 by Mike Walters

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/Debounce
 */

class Button
{
public:
  Button( int pin )
  : _pin( pin ), _lastButtonState( LOW ),
  _lastDebounceTime( 0 ), _debounceDelay( 50 )
  {

  }

  void setup() {
    pinMode( _pin, INPUT );
  }

  void loop() {
    // reset button press flags
    _didRelease = false;
    _didPress = false;

    // read the state of the switch into a local variable:
    int reading = digitalRead( _pin );

    // check to see if you just pressed the button
    // (i.e. the input went from LOW to HIGH),  and you've waited
    // long enough since the last press to ignore any noise:

    // If the switch changed, due to noise or pressing:
    if ( reading != _lastButtonState ) {
      // reset the debouncing timer
      _lastDebounceTime = millis();
    }

    if ( ( millis() - _lastDebounceTime ) > _debounceDelay ) {
      // whatever the reading is at, it's been there for longer
      // than the debounce delay, so take it as the actual current state:

      // if the button state has changed:
      if ( reading != _buttonState ) {
        _buttonState = reading;

        // only toggle the LED if the new button state is HIGH
        if ( _buttonState == HIGH ) {
          // off
          _didRelease = true;
        } else {
          // on
          _didPress = true;
        }

      }
    }

    // save the reading.  Next time through the loop,
    // it'll be the lastButtonState:
    _lastButtonState = reading;
  }

  bool isOn() { return _buttonState == LOW; }
  bool didPress() { return _didPress; }
  bool didRelease() { return _didRelease; }

private:
  int _pin; // button pin

  int _buttonState;       // the current reading from the input pin
  int _lastButtonState;   // the previous reading from the input pin

  // the following variables are long's because the time, measured in miliseconds,
  // will quickly become a bigger number than can be stored in an int.
  unsigned long _lastDebounceTime; // the last time the output pin was toggled
  unsigned long _debounceDelay;    // the debounce time; increase if the output flickers

  bool _didPress; // was the button just pressed down?
  bool _didRelease; // was the button just released?
};
