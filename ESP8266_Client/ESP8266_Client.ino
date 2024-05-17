#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
 #include <SoftwareSerial.h>


// Set WiFi credentials
#define WIFI_SSID "WSESP8266AP"
#define WIFI_PASS "password"

SoftwareSerial mySerial(D4, D5); // RX, TX
// D4 RX - PA9
// D5 TX - PA10
// (Send and Receive)

// UDP
WiFiUDP UDP;
IPAddress remote_IP(192,168,4,1);
#define UDP_PORT 4210

void setup() {

  // Setup IO
  pinMode(2, INPUT);
  
  // Setup serial port
  Serial.begin(115200);
  mySerial.begin(115200);
  Serial.println();
 
  // Begin WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  WiFi.mode(WIFI_STA);
 
  // Connecting to WiFi...
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  // Loop continuously while WiFi is not connected
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
  }
 
  // Connected to WiFi
  Serial.println();
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  // Begin UDP port
  UDP.begin(UDP_PORT);
  Serial.print("Opening UDP port ");
  Serial.println(UDP_PORT);
 
}
 

void connect(){

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  WiFi.mode(WIFI_STA);
 
  // Connecting to WiFi...
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  // Loop continuously while WiFi is not connected
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
  }
 
  // Connected to WiFi
  Serial.println();
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  // Begin UDP port
  UDP.begin(UDP_PORT);
  Serial.print("Opening UDP port ");
  Serial.println(UDP_PORT);
 

}

void loop() {

  ////////////////////////// data IN ////////////////////////////
    // Read from the serial monitor and send to mySerial
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n'); // Read until newline
    mySerial.println(input);
  }
  
  // Read from mySerial and send to the serial monitor
  if (mySerial.available() >= 2) { // Check if at least 2 bytes are available
    uint8_t byte1 = mySerial.read(); // Read the first byte
    uint8_t byte2 = mySerial.read(); // Read the second byte

    // Print the bytes in hexadecimal format
    Serial.print("Received bytes: ");
    Serial.print(byte1, HEX);
    Serial.print(" ");
    Serial.println(byte2, HEX);
    ////////////////////////// data IN ////////////////////////////

  // Read button
  char buttonState = digitalRead(2);
  int DataIn = mySerial.read();
  // Send Packet
  UDP.beginPacket(remote_IP, UDP_PORT);
  UDP.write(byte1); // << 2 wide packet
  UDP.write(byte2);
  UDP.endPacket();
  /*
  UDP.beginPacket(remote_IP, UDP_PORT);
  UDP.write(byte1);
  UDP.endPacket();
  UDP.beginPacket(remote_IP, UDP_PORT);
  UDP.write(byte2);
  UDP.endPacket();
  */
  delay(100);
  if (WiFi.status() != WL_CONNECTED)
  {
    delay(1);
    Serial.print("AP Disconnected!\n");
    connect();
  }

  }
}