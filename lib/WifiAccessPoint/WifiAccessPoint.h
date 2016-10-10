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
    HTTP_req += c;  // save the HTTP request 1 char at a time
// last line of client request is blank and ends with \n
// respond to client only after last line received
if (c == '\n' && currentLineIsBlank) {
    // send a standard http response header
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: keep-alive");
    client.println();
    counter = 0;
    // AJAX request for switch state
    if (HTTP_req.indexOf("ajax_switch") > -1) {
        // read switch state and analog input
        getAjaxData(client);
        counter = 0;
        yield();
    }
    else {  // HTTP request for web page
        // send web page - contains JavaScript with AJAX calls
        client.println("<!DOCTYPE html>");
        client.println("<html>");
        client.println("<head>");
        client.println("<title>Arduino Web Page</title>");
        client.println("<script>");
        client.println("function GetSwitchAnalogData() {");
        client.println(
            "nocache = \"&nocache=\" + Math.random() * 1000000;");
        client.println("var request = new XMLHttpRequest();");
        client.println("request.onreadystatechange = function() {");
        client.println("if (this.readyState == 4) {");
        client.println("if (this.status == 200) {");
        client.println("if (this.responseText != null) {");
        client.println("document.getElementById(\"sw_an_data\")\
.innerHTML = this.responseText;");
        client.println("}}}}");
        client.println(
        "request.open(\"GET\", \"ajax_switch\" + nocache, true);");
        client.println("request.send(null);");
        client.println("setTimeout('GetSwitchAnalogData()', 200);");
        client.println("}");
        client.println("</script>");
        client.println("</head>");
        client.println("<body onload=\"GetSwitchAnalogData()\">");
        client.println("<h1>Arduino AJAX Input</h1>");
        client.println("<div id=\"sw_an_data\">");
        client.println("</div>");
        client.println("</body>");
        client.println("</html>");
    }
    // display received HTTP request on serial port
    //Serial.print(HTTP_req);
    HTTP_req = "";            // finished with request, empty string
    counter = 0;
    break;
}
// every line of text received from the client ends with \r\n
if (c == '\n') {
      // last character on line of received text
    // starting new line with next character read
    currentLineIsBlank = true;
    counter = 0;
} else if (c != '\r') {
    // a text character was received from client
    currentLineIsBlank = false;
    counter = 0;
}
} else if (counter > 5) {
  counter = 0;
  break;
} else {
  counter++;
} // end if (client.available())
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
  cl.print("Is Running: "); cl.print(isCurrentlyRunning);
  cl.println("<br>");
  cl.print("Throttle: "); cl.print(throttle);
  cl.println("<br>");
  cl.print("Steering: "); cl.print(steering);
  cl.println("<br>");
  cl.print("Current Heading: "); cl.print(currentHeading);
  cl.println("<br>");
  cl.print("Wanted Heading:  "); cl.print(wantedHeading);
  cl.println("<br>");


}

void updateInformation(int throttles, int steerings, float currentHeadings, float wantedHeadings, bool isRunning) {
  throttle = throttles;
  steering = steerings;
  currentHeading = currentHeadings;
  wantedHeading = wantedHeadings;
  isCurrentlyRunning = isRunning;
}

private:
  String HTTP_req;
  int counter = 0;
  int status = WL_IDLE_STATUS;
  int throttle = 0;
  int steering = 0;
  float currentHeading = 0;
  float wantedHeading = 0;
  bool isCurrentlyRunning;
};
