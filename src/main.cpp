
#include <Arduino.h>
#include <WiFi.h>

// Replace with your network credentials 
const char* ssid = "Redmi"; 
const char* password = "12345678"; 

// Set web server port number to 80 
WiFiServer server(80);

// Variable to store the HTTP request String header;
String header;

// Declare the pins to which the LEDs are connected 
int greenled = 26;
int redled = 27; 

String greenstate = "OFF";// state of green LED
String redstate = "OFF";// state of red LED

//NTC
int ThermistorPin = A0;
int Vo;
float R1 = 10000;
float logR2, R2, T, Tc, Tf;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
String TempC ="0";

void setup() {
  Serial.begin(115200);
 // Set the pinmode of the pins to which the LEDs are connected and turn them low to prevent flunctuations
  pinMode(ThermistorPin, INPUT);
  pinMode(greenled, OUTPUT);
  pinMode(redled, OUTPUT);
  digitalWrite(greenled, LOW);
  digitalWrite(redled, LOW);
  //connect to access point
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());// this will display the Ip address of the Pi which should be entered into your browser
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
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
            if (header.indexOf("GET /green/on") >= 0) {
              Serial.println("green on");
              greenstate = "ON";
              digitalWrite(greenled, HIGH);
            } else if (header.indexOf("GET /green/off") >= 0) {
              Serial.println("green off");
              greenstate = "OFF";
              digitalWrite(greenled, LOW);
            } else if (header.indexOf("GET /red/on") >= 0) {
              Serial.println("red on");
              redstate = "ON";
              digitalWrite(redled, HIGH);
            } else if (header.indexOf("GET /red/off") >= 0) {
              Serial.println("red off");
              redstate = "OFF";
              digitalWrite(redled, LOW);
            }

            else if (header.indexOf("GET /") >= 0) {
               //NTC
                Vo = analogRead(ThermistorPin);
                R2 = R1 * (4095.0 / (float)Vo - 1.0);
                logR2 = log(R2);
                T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
                Tc = T - 273.15;
                Tf = (Tc * 9.0)/ 5.0 + 32.0;
                TempC = String(Tc);}

            // Edit Display the HTML web page
            client.println("<!DOCTYPE html>");
            client.println("<html>");
            //head.begin
            client.println("<head>");
            client.println("<title>WEB SERVER CONTROL</title>");
            //--CSS--
            client.println("<style>");
            //html
            client.println("html {");
            client.println("font-family: Helvetica,Arial,sans-serif;");
            client.println("display: inline-block;");
            client.println("margin: 0px auto;");
            client.println("text-align:center;");
            client.println("background-image: linear-gradient(to bottom right, #2a93d5, #ffffff);");
            client.println("}");
            
            //Button
            client.println("button {");
            client.println("background-color: #b0d12a;");
            client.println("border: 2px solid #422800;");
            client.println("border-radius: 30px;");
            client.println("box-shadow: #422800 4px 4px 0 0;");
            client.println("color: #422800;");
            client.println("cursor: pointer;");
            client.println("display: inline-block;");
            client.println("font-weight: 600;");
            client.println("font-size: 18px;");
            client.println("padding: 0px 50px;");
            client.println("line-height: 50px;");
            client.println("text-align: center;");
            client.println("text-decoration: none;");
            client.println("user-select: none;");
            client.println("-webkit-user-select: none;");
            client.println("touch-action: manipulation;");
            client.println("}");
            //button2
            client.println(".button2 {");
            client.println("background-color: #b92e34;");
            client.println("}");
            //div
            client.println("div {");
            client.println("font-family:Helvetica;");
            client.println("font-size: 60px;");
            client.println("line-height: .93;");
            client.println("letter-spacing: -0.015em;");
            client.println("text-align: left;");
            client.println("position: relative;");
            client.println("top:-50px;");
            client.println("left: 100px;");
            client.println("width:100%;");
            client.println("height: 100%;");
            client.println("}");
            //table
            client.println("table{");
            client.println("width: 900px;");
            client.println("height: -300px;");
            client.println("}");
            //div1
            client.println("div1{");
            client.println("font-size: 35px;");
            client.println("line-height: .93;");
            client.println("font-weight: 600;");
            client.println("letter-spacing: -0.015em;");
            client.println("text-align: left;");
            client.println("position: relative;");
            client.println("top:-50px;");
            client.println("left: -100px;");
            client.println("width:100%;");
            client.println("height: 100%;");
            client.println("</style>");
            client.println("</head>");
            //head.end
          
            //body.begin
            client.println("<body>");
            //div
            client.println("<div>");
            client.println("<h1>Smart<br>Home Control</h1> ");
            client.println("<div>");
            
            client.println("<br> ");

            //div1
            client.println("<div1>");
            client.println("<table>");
            client.println("<thead> ");
            client.println("<tr> ");
            client.println("<td><p>Switch 1 - " + greenstate + "</p></td> ");
            client.println("<td><p>Switch 2 - " + redstate + "</p></td> ");
            client.println("<td>Temperature(C)</td> ");
            client.println("</tr> ");
            client.println("</thead> ");
            client.println("<tbody> ");
            client.println("<tr style=\"position:relative;left:55px;\"> ");
             
             // Display current state, and ON/OFF buttons for GPIO 26
             if (greenstate == "OFF") {
            client.println("<td><p><a href=\"/green/on\"><button class=\"button\">ON</button></a></p></td>");
             } else {
              client.println("<td><p><a href=\"/green/off\"><button class=\"button button2\">OFF</button></a></p></td>");
             }

             // Display current state, and ON/OFF buttons for GPIO 27
             if (redstate == "OFF") {
            client.println("<td><p><a href=\"/red/on\"><button class=\"button\">ON</button></a></p></td> ");
             } else {
              client.println("<td><p><a href=\"/red/off\"><button class=\"button button2\">OFF</button></a></p></td> ");
             }
             // Measure Temperature
            client.print("<td>");
            client.print(TempC);
            client.println("</td> ");
            client.println("</tr> ");
            client.println("</tbody> ");
            client.println("</table> ");
            client.println("</div1> ");
            client.println("<br> ");
            
            client.println("</body>");
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
    Serial.println(Tc);
    
  }
}

