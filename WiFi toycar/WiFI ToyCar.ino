#include <ESP8266WiFi.h>

const char* ssid     = "Bethel_2.4G"; //Check HGU_WLAN
const char* password = "handong1"; //Check your password

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// variables to store the current output state
String output7State = "stop";

/* define L298N or L293D motor control pins */
//const int output7 = 13 ; //output connected to 7
const int leftMotorBackward = 0;    /* GPIO0(D3) -> IN4   */
const int leftMotorForward = 2;     /* GPIO2(D4) -> IN3   */
const int rightMotorForward = 15;   /* GPIO15(D8) -> IN1  */
const int rightMotorBackward = 13;  /* GPIO13(D7) -> IN2  */

/* define L298N or L293D enable pins */
int rightMotorENB = 14; /* GPIO14(D5) -> Motor-A Enable */
int leftMotorENB = 12;  /* GPIO12(D6) -> Motor-B Enable */

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(leftMotorForward, OUTPUT);
  pinMode(rightMotorForward, OUTPUT); 
  pinMode(leftMotorBackward, OUTPUT);  
  pinMode(rightMotorBackward, OUTPUT);

  pinMode(leftMotorENB, OUTPUT); 
  pinMode(rightMotorENB, OUTPUT);
  
  //pinMode(output4, OUTPUT);
//  digitalWrite(output7, LOW);
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
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
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /5/forward") >= 0) {
              Serial.println("Motor forward");
              output7State = "forward";
              MotorForward();
            } else if (header.indexOf("GET /5/backward") >= 0) {
              Serial.println("Motor backward");
              output7State = "backward";
              MotorBackward();
            }
            else if (header.indexOf("GET /5/left") >= 0) {
              Serial.println("Motor left");
              output7State = "left";
              TurnLeft();
            }
            else if (header.indexOf("GET /5/right") >= 0) {
              Serial.println("Motor right");
              output7State = "right";
              TurnRight();
            }
            else if (header.indexOf("GET /5/stop") >= 0) {
              Serial.println("Motor stop");
              output7State = "stop";
              MotorStop();
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: yellow;font-size: 30px; ");
            client.println("text-decoration: none;}");
            client.println(".button2 {background-color: gray;font-size: 30px;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h2>IOT FINAL EXAM</h2>");
            
            // Display current state, and ON/OFF buttons for GPIO 5  
            client.println("<p><h3>Toy car ESP8266 Wifi controller<h3></p>");
            // If the output7State is off, it displays the ON button   
            
            if (output7State=="forward") {
              client.println("<p><a href=\"/5/forward\"><button class=\"button2\">Forward</button></a></p>"); 
            client.println("<p><a href=\"/5/backward\"><button class=\"button\">Backward</button></a></p>");  
            client.println("<p><a href=\"/5/left\"><button class=\"button\">Turn left</button></a></p>"); 
            client.println("<p><a href=\"/5/right\"><button class=\"button\">Turn right</button></a></p>");
            client.println("<p><a href=\"/5/stop\"><button class=\"button\">Stop</button></a></p>");
              }
            else if (output7State=="backward") {
              client.println("<p><a href=\"/5/forward\"><button class=\"button\">Forward</button></a></p>"); 
            client.println("<p><a href=\"/5/backward\"><button class=\"button2\">Backward</button></a></p>");  
            client.println("<p><a href=\"/5/left\"><button class=\"button\">Turn left</button></a></p>"); 
            client.println("<p><a href=\"/5/right\"><button class=\"button\">Turn right</button></a></p>");
            client.println("<p><a href=\"/5/stop\"><button class=\"button\">Stop</button></a></p>");
              } 
            else if (output7State=="left"){
              client.println("<p><a href=\"/5/forward\"><button class=\"button\">Forward</button></a></p>"); 
            client.println("<p><a href=\"/5/backward\"><button class=\"button\">Backward</button></a></p>");  
            client.println("<p><a href=\"/5/left\"><button class=\"button2\">Turn left</button></a></p>"); 
            client.println("<p><a href=\"/5/right\"><button class=\"button\">Turn right</button></a></p>");
            client.println("<p><a href=\"/5/stop\"><button class=\"button\">Stop</button></a></p>");
              }
            else if (output7State=="right"){
              client.println("<p><a href=\"/5/forward\"><button class=\"button\">Forward</button></a></p>"); 
            client.println("<p><a href=\"/5/backward\"><button class=\"button\">Backward</button></a></p>");  
            client.println("<p><a href=\"/5/left\"><button class=\"button\">Turn left</button></a></p>"); 
            client.println("<p><a href=\"/5/right\"><button class=\"button2\">Turn right</button></a></p>");
            client.println("<p><a href=\"/5/stop\"><button class=\"button\">Stop</button></a></p>");
              }
            else if (output7State=="stop"){
              client.println("<p><a href=\"/5/forward\"><button class=\"button\">Forward</button></a></p>"); 
            client.println("<p><a href=\"/5/backward\"><button class=\"button\">Backward</button></a></p>");  
            client.println("<p><a href=\"/5/left\"><button class=\"button\">Turn left</button></a></p>"); 
            client.println("<p><a href=\"/5/right\"><button class=\"button\">Turn right</button></a></p>");
            client.println("<p><a href=\"/5/stop\"><button class=\"button2\">Stop</button></a></p>");
              }
            client.println("<p><h3>Renaud Axel Monentchame Eba - 22147003<h3></p>");          
            client.println("</body></html>");
            
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
/********************************************* FORWARD *****************************************************/
void MotorForward()   
{
  digitalWrite(leftMotorENB,HIGH);
  digitalWrite(rightMotorENB,HIGH);
  digitalWrite(rightMotorForward,HIGH);  
  digitalWrite(rightMotorBackward,LOW);     
    digitalWrite(leftMotorForward,HIGH); 
  digitalWrite(leftMotorBackward,LOW);
 
}

/********************************************* BACKWARD *****************************************************/
void MotorBackward()   
{
  digitalWrite(leftMotorENB,HIGH);
  digitalWrite(rightMotorENB,HIGH);
  digitalWrite(leftMotorBackward,HIGH);
  digitalWrite(rightMotorBackward,HIGH);
  digitalWrite(leftMotorForward,LOW);
  digitalWrite(rightMotorForward,LOW);
}

/********************************************* TURN LEFT *****************************************************/
void TurnLeft()   
{
  digitalWrite(leftMotorENB,HIGH);
  digitalWrite(rightMotorENB,HIGH); 
  digitalWrite(leftMotorForward,LOW);
  digitalWrite(rightMotorForward,HIGH);
  digitalWrite(rightMotorBackward,LOW);
  digitalWrite(leftMotorBackward,LOW);  
}

/********************************************* TURN RIGHT *****************************************************/
void TurnRight()   
{
  digitalWrite(leftMotorENB,HIGH);
  digitalWrite(rightMotorENB,HIGH);
  digitalWrite(leftMotorForward,HIGH);
  digitalWrite(rightMotorForward,LOW);
  digitalWrite(rightMotorBackward,LOW);
  digitalWrite(leftMotorBackward,LOW);
}

/********************************************* STOP *****************************************************/
void MotorStop()   
{
  digitalWrite(leftMotorENB,LOW);
  digitalWrite(rightMotorENB,LOW);
  digitalWrite(leftMotorForward,LOW);
  digitalWrite(leftMotorBackward,LOW);
  digitalWrite(rightMotorForward,LOW);
  digitalWrite(rightMotorBackward,LOW);
}
