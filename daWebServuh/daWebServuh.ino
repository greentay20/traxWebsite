/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com
*********/

// Load Wi-Fi library

#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h> //importing the Servo library to be able to use the servo
Servo servo1; //creating servo object

// Replace with your network credentials
// const char* ssid     = "US283X";
// const char* password = "smartwifi";
const char* ssid     = "SHS_Guest";
//const char* password = "";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output5State = "off";
String output4State = "off";


// Assign output variables to GPIO pins
const int output5 = 5;
const int output4 = 4;
const int trigPin = 14; //Initializing trigger pin
const int echoPin = 12; //Initializing echo pin
long duration;
int distance;
// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;


void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(output5, OUTPUT);
  pinMode(output4, OUTPUT);
  //sets up pins for sonar
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  // Set outputs to LOW
  digitalWrite(output5, LOW);
  digitalWrite(output4, LOW);
  // Servo:
  servo1.attach(9); //initializing servo1 to pin 9

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  //WiFi.begin(ssid, password);
  WiFi.begin(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

 // setting static IP
 /*
  IPAddress ip(172,16,1,3);
  IPAddress gateway(172,16,1,1);
  IPAddress subnet(255,255,255,0);
  WiFi.config(ip, gateway, subnet);
 */

  const char* deviceName = "knacknotron.com";
  
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
wifi();
distance = Distance(); //
Serial.println(distance);
Serial.println("");
delay(500);
}

int Distance() // Function for calculating the distance measured by the Ultrasonic sensor
{
  digitalWrite(trigPin, LOW);  // Sets the trigPin on LOW state for 2 micro seconds
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); // Sets the trigPin on HIGH state for 10 micro seconds
  delayMicroseconds(10);
  digitalWrite(9, LOW);
  delayMicroseconds(2);
  duration = pulseIn(echoPin, HIGH); // Reads the echoPin, returns the sound wave travel time in microseconds
  distance= duration*0.034/2; //Converting distance into meters
  return distance;
}
void wifi() {
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            /*
            // turns the GPIOs on and off
            if (header.indexOf("GET /Servo/on") >= 0) {
              Serial.println("Servo on");
              output5State = "on";
              digitalWrite(output5, HIGH);
              Serial.println("Servo value: 180"); //printing the servo value to the serial to monitor it
              servo1.write(180); //moving the servo to 180
            } else if (header.indexOf("GET /Servo/off") >= 0) {
              Serial.println("Servo off");
              output5State = "off";
              digitalWrite(output5, LOW);
              Serial.println("Servo value: 0"); //printing the servo value to the serial to monitor it
              servo1.write(0); //moving the servo to 0
              } else if (header.indexOf("GET /4/on") >= 0) {
              Serial.println("GPIO 4 on");
              output4State = "on";
              digitalWrite(output4, HIGH);
            } else if (header.indexOf("GET /4/off") >= 0) {
              Serial.println("GPIO 4 off");
              output4State = "off";
              digitalWrite(output4, LOW);
            } */

            // Display the HTML web page
            client.println("<!DOCTYPE html>");
            client.println("<html lang=\"en\" dir=\"ltr\">");
            client.println("  <head>");
            client.println("    <meta charset=\"utf-8\">");
            client.println("    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
            client.println("    <meta http-equiv=\"X-UA-Compatible\" content=\"ie=edge\">");
            client.println("<style>");
            client.println("body {");
            client.println("    margin: 0; padding: 5em;");
            client.println("    box-sizing: border-box;");
            client.println("    width: 100%;");
            client.println("    height: 100%;");
            client.println("    background: #b3c6ff;");
            client.println("}");
            client.println("#wave {");
            client.println("  width: 100%;");
            client.println("  height: 100%;");
            client.println("  top: 0;");
            client.println("  left: 0;");
            client.println("  position: absolute;");
            client.println("  pointer-events: none;");
            client.println("}");
            client.println("#letT {");
            client.println("  stroke-dasharray: 2000;");
            client.println("  stroke-dashoffset: 0;");
            client.println("  animation: dash 4s linear forwards;");
            client.println("  width: 100%;");
            client.println("  height: 100%;");
            client.println("  top: 0;");
            client.println("  left: 0;");
            client.println("  position: absolute;");
            client.println("  pointer-events: none;");
            client.println("}");
            client.println("@keyframes dash {");
            client.println("  from {");
            client.println("    stroke-dashoffset: 2000;");
            client.println("  }");
            client.println("  to {");
            client.println("    stroke-dashoffset: 0;");
            client.println("  }");
            client.println("}");
            client.println("#letR {");
            client.println("  stroke-dasharray: 2000;");
            client.println("  stroke-dashoffset: 0;");
            client.println("  animation: dash 4s linear forwards;");
            client.println("  width: 100%;");
            client.println("  height: 100%;");
            client.println("  top: 0;");
            client.println("  left: 0;");
            client.println("  position: absolute;");
            client.println("  pointer-events: none;");
            client.println("}");
            client.println("@keyframes dash {");
            client.println("  from {");
            client.println("    stroke-dashoffset: 2000;");
            client.println("  }");
            client.println("  to {");
            client.println("    stroke-dashoffset: 0;");
            client.println("  }");
            client.println("}");
            client.println("#letA {");
            client.println("  stroke-dasharray: 2000;");
            client.println("  stroke-dashoffset: 0;");
            client.println("  animation: dash 4s linear forwards;");
            client.println("  width: 100%;");
            client.println("  height: 100%;");
            client.println("  top: 0;");
            client.println("  left: 0;");
            client.println("  position: absolute;");
            client.println("  pointer-events: none;");
            client.println("}");
            client.println("@keyframes dash {");
            client.println("  from {");
            client.println("    stroke-dashoffset: 2000;");
            client.println("  }");
            client.println("  to {");
            client.println("    stroke-dashoffset: 0;");
            client.println("  }");
            client.println("}");
            client.println("#letX {");
            client.println("  stroke-dasharray: 2000;");
            client.println("  stroke-dashoffset: 0;");
            client.println("  animation: dash 4s linear forwards;");
            client.println("  width: 100%;");
            client.println("  height: 100%;");
            client.println("  top: 0;");
            client.println("  left: 0;");
            client.println("  position: absolute;");
            client.println("  pointer-events: none;");
            client.println("}");
            client.println("@keyframes dash {");
            client.println("  from {");
            client.println("    stroke-dashoffset: 2000;");
            client.println("  }");
            client.println("  to {");
            client.println("    stroke-dashoffset: 0;");
            client.println("  }");
            client.println("}");
            client.println(".button {");
            client.println("  background-color: #b3c6ff;");
            client.println("  color: white;");
            client.println("  position: fixed;");
            client.println("  padding: 20px 40px;");
            client.println("  top: 75%;");
            client.println("  left: 50%;");
            client.println("  transform: translate(-50%, -50%);");
            client.println("  border: 2px solid white;");
            client.println("  border-radius: 40px;");
            client.println("  text-align: center;");
            client.println("  text-decoration: none;");
            client.println("  font-size: 20px;");
            client.println("  display: block;");
            client.println("  cursor: pointer;");
            client.println("}");
            client.println("</style>");
            client.println("    <title>Trax</title>");
            client.println("    <!-- <script src=\"https://cdnjs.cloudflare.com/ajax/libs/animejs/2.2.0/anime.js\"></script>");
            client.println("    <script src=\"main.js\"></script>");
            client.println("    <link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/animate.css/3.7.2/animate.min.css\"> -->");
            client.println("  </head>");
            client.println("  <body>");
            client.println("    <!-- Adding the wave to page -->");
            client.println("    <svg id=\"wave\" height=\"100%\" width=\"100%\" viewBox=\"0 0 1920 1080\" preserveAspectRatio=\"none\">");
            client.println("        <path class=\"wave\" d=\"M0,0V275.56S99.93,86.133,309.756,84.95c65.2-.367,151.491,13.58,235.246,61.936,125.807,72.634,185.346,161.948,292.381,156.038,193-10.656,388.356-214.926,513.578-181.373C1443.711,146.4,1520,237.9,1634.283,232.277,1758.84,226.152,1917.5,121.551,1917.5,121.551V0Z\" stroke = \"#ffffb3\" stroke-width = \"1.5\" fill=\"#ccb3ff\"></path>");
            client.println("    </svg>");
            client.println("    <!-- Adding the letter \"t\" to page -->");
            client.println("    <svg id=\"letT\" height=\"100%\" width=\"100%\" viewBox=\"0 0 1920 1080\" preserveAspectRatio=\"none\">");
            client.println("        <path class=\"letT\" d=\"M489.55,412v47.45h61.91v35h-61.91v44.19c0,5.31,1.2,8.83,3.59,10.55c3.68,2.66,10.13,3.99,19.35,3.99c8.29,0,19.9-1.02,34.82-3.05l8.29,33c-27.82,2.58-53.8,3.88-77.94,3.88c-28.01,0-48.64-1.53-61.91-4.58c-13.27-3.05-23.08-7.68-29.43-13.9c-6.36-6.22-9.54-16.29-9.54-30.2v-43.87h-41.46v-35h41.46v-22.9L489.55,412z\" stroke=\"#ffffff\" stroke-width=\"6\" fill=transparent></path>");
            client.println("    </svg>");
            client.println("    <!-- Adding the letter \"r\" to page -->");
            client.println("    <svg id=\"letR\" height=\"100%\" width=\"100%\" viewBox=\"0 0 1920 1080\" preserveAspectRatio=\"none\">");
            client.println("        <path class=\"letR\" d=\"M606.18,459.45h105.3v20.44c10.13-8.85,20.59-14.93,31.37-18.26c10.78-3.33,24.09-4.99,39.94-4.99c16.58,0,34.73,2.19,54.45,6.58l-34.82,34.06c-13.27-2.35-23.77-3.52-31.51-3.52c-14.74,0-26.17,2.58-34.27,7.75c-11.61,7.28-17.41,20.91-17.41,40.87v41.81H606.18V459.45z\" stroke=\"#ffffff\" stroke-width=\"6\" fill=transparent></path>");
            client.println("    </svg>");
            client.println("    <!-- Adding the letter \"a\" to page -->");
            client.println("    <svg id=\"letA\" height=\"100%\" width=\"100%\" viewBox=\"0 0 1920 1080\" preserveAspectRatio=\"none\">");
            client.println("        <path class=\"letA\" d=\"M968.51,499.85L861,495.04c4.05-7.99,9.9-14.27,17.55-18.85c7.64-4.58,18.66-8.55,33.03-11.92c10.32-2.43,24.5-4.31,42.56-5.64c18.05-1.33,37.59-2,58.59-2c33.72,0,60.8,0.8,81.25,2.41c20.45,1.61,37.49,4.95,51.13,10.04c9.58,3.52,17.14,8.52,22.66,14.97c5.53,6.46,8.29,12.63,8.29,18.5v55.08c0,5.87,0.87,10.47,2.63,13.8c1.75,3.33,5.57,7.58,11.47,12.74h-105.58c-4.24-3.21-7-5.66-8.29-7.34c-1.29-1.68-2.58-4.32-3.87-7.93c-14.74,6.03-29.39,10.34-43.94,12.92c-19.9,3.44-43.02,5.17-69.37,5.17c-35.01,0-61.59-3.44-79.73-10.34c-18.15-6.89-27.22-15.39-27.22-25.49c0-9.47,6.54-17.27,19.62-23.37c13.08-6.11,37.22-10.65,72.41-13.62c42.19-3.6,69.55-6.13,82.08-7.58c12.53-1.45,25.79-3.35,39.8-5.7c0-5.87-2.86-9.98-8.57-12.33c-5.71-2.35-15.75-3.52-30.13-3.52c-18.43,0-32.25,1.25-41.46,3.76C978.73,490.77,972.93,494.45,968.51,499.85z M1066.07,524.99c-15.48,2.35-31.6,4.42-48.37,6.22c-22.85,2.58-37.31,5.13-43.39,7.63c-6.27,2.58-9.4,5.52-9.4,8.81c0,3.76,3.08,6.83,9.26,9.22c6.17,2.39,15.24,3.58,27.22,3.58c12.53,0,24.18-1.29,34.96-3.88c10.78-2.58,18.42-5.74,22.94-9.46c4.51-3.72,6.77-8.55,6.77-14.5V524.99z\" stroke=\"#ffffff\" stroke-width=\"6\" fill=transparent></path>");
            client.println("    </svg>");
            client.println("    <!-- Adding the letter \"x\" to page -->");
            client.println("    <svg id=\"letX\" height=\"100%\" width=\"100%\" viewBox=\"0 0 1920 1080\" preserveAspectRatio=\"none\">");
            client.println("        <path class=\"letX\" d=\"M1218.35,459.45h133.49l46.57,34.76l54.31-34.76h124.09l-100.13,59.43l107.31,65.3h-131.28l-54.31-40.17l-63.7,40.17h-121.88l106.6-65.3L1218.35,459.45z\" stroke=\"#ffffff\" stroke-width = \"6\" fill=transparent></path>");
            client.println("    </svg>");
            client.println("    <a href = \"traxMap.html\"><button class=\"button controlButton\">Controls</button></a>");
            client.println("  </body>");
            client.println("</html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
