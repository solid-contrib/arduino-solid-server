#include <DallasTemperature.h>

/*
  Web Server
 
 A simple web server that shows the value of the analog input pins, and temperature fromoine-wire sensors.
 using an Arduino Wiznet Ethernet shield. 
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)
 
2009-12-18 created 18 Dec 2009 by David A. Mellis
2012  modified 9 Apr 2012 by Tom Igoe
2014  digital pins added and convert to turtle -- timbl 
 */

#include <SPI.h>
#include <Ethernet.h>

#include <OneWire.h>
#include <DallasTemperature.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,0,20);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);


// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 3

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

 



void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());

  sensors.begin();

}


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          // client.println("Content-Type: text/html");
          client.println("Content-Type: text/turtle");
          client.println("Connection: close");  // the connection will be closed after completion of the response
	  client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          // client.println("<!DOCTYPE HTML>");
          // client.println("<html><pre>");
          
          // output the value of each analog input pin


          client.println("@prefix : <http://www.w3.org/ns/pim/arduino#>.\n");
          client.println("@prefix this: <#>.\n\n");
          client.println("this:thisDevice :state [");



          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            int sensorReading = analogRead(analogChannel);
            client.print(":a");
            client.print(analogChannel);
            client.print(" ");
            client.print(sensorReading);
            client.println(";");       
          }

           client.println("");       

// digitalRead
          for (int pin = 0; pin < 12; pin++) {
            int sensorReading = digitalRead(pin);
            client.print(":d");
            client.print(pin);
            client.print(" ");
            client.print(sensorReading);
            client.println(";");      // was: if (pin != 11) 
          }

          sensors.requestTemperatures(); // Send the command to get temperatures
          
          int deviceCount = sensors.getDeviceCount();
          for (int dev = 0; dev < deviceCount; dev++) {
            client.print(":t");
            client.print(dev);
            client.print(" ");
            client.print(sensors.getTempCByIndex(dev));
            if (dev < deviceCount - 1) {
                 client.println(";");    
            }
          }
          client.println("\n].\n");
          // client.println(\n</html>");
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
  }
}
