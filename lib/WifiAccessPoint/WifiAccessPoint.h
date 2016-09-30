#include <config.h>
#include <WiFi101.h>

  WiFiServer server(80);

class WifiAccessPoint {
public:
  WifiAccessPoint(){}

  void setup() {
    Serial.begin(9600);

    status = WiFi.beginAP(ssid);

    if (status != WL_AP_LISTENING) {
      Serial.println("Creating access point failed");
      while(true){//Do not continue  print out LED code
        digitalWrite(LED_BUILTIN, HIGH);
        delay(1000);
        digitalWrite(LED_BUILTIN, LOW);
        delay(1000);
      }
    }

    server.begin();
  }

  void loop(){
    // compare the previous status to the current status
  if (status != WiFi.status()) {
    // it has changed update the variable
    status = WiFi.status();

    if (status == WL_AP_CONNECTED) {
      // a device has connected to the AP
      Serial.println("Device connected to AP");
    } else {
      // a device has disconnected from the AP, and we are back in listening mode
      Serial.println("Device disconnected from AP");
    }
  }

  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {  // got client?
    // an http request ends with a blank line
  boolean currentLineIsBlank = true;
  while (client.connected()) {
    digitalWrite(LED_BUILTIN, HIGH);
    if (client.available()) {
      char c = client.read();
      //Serial.write(c);
      // if you've gotten to the end of the line (received a newline
      // character) and the line is blank, the http request has ended,
      // so you can send a reply
      if (c == '\n' && currentLineIsBlank) {
        // send a standard http response header
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println("Connection: close");  // the connection will be closed after completion of the response
        client.println("Refresh: 1");  // refresh the page automatically every 5 sec
        client.println();
        client.println("<!DOCTYPE HTML>");
        client.println("<html>");
        // output the value of each analog input pin

        client.print("Is it running:  "); client.print(isCurrentlyRunning);
        client.println("<br>");
        client.print("Throttle: "); client.print(throttle);
        client.println("<br>");
        client.print("Steering: "); client.print(steering);
        client.println("<br>");
        client.print("Current Heading: "); client.print(currentHeading);
        client.println("<br>");
        client.print("Wanted Heading:  "); client.print(wantedHeading);
        client.println("<br>");

        client.println("</html>");
        client.println();
        break;
      }
      if (c == '\n') {
        // you're starting a new line
        currentLineIsBlank = true;
      }
      else if (c != '\r') {
        // you've gotten a character on the current line
        currentLineIsBlank = false;
      }
    }
  }
  // give the web browser time to receive the data
  delay(1);

  // close the connection:
  client.stop();
  Serial.println("client disconnected");
  digitalWrite(LED_BUILTIN, LOW);
}
}

// send the state of the switch to the web browser
void getAjaxData(WiFiClient cl) {
  cl.print("Throttle: "); cl.print(throttle);
  cl.print("Steering: "); cl.print(steering);
  cl.print("Current Heading: "); cl.print(currentHeading);
  cl.print("Wanted Heading:  "); cl.print(wantedHeading);


}

void updateInformation(int throttles, int steerings, double currentHeadings, double wantedHeadings, bool isRunning) {
  throttle = throttles;
  steering = steerings;
  currentHeading = currentHeadings;
  wantedHeading = wantedHeadings;
  isCurrentlyRunning = isRunning;
}

private:
  int status = WL_IDLE_STATUS;
  int throttle = 0;
  int steering = 0;
  double currentHeading = 0;
  double wantedHeading = 0;
  bool isCurrentlyRunning;
};
