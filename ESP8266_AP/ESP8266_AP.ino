#include <ESP8266WiFi.h>
#include <WiFiUdp.h>


// Defines
/*
We will define the constant AD_SSID, which will contain the name of our ESP8266 router.
*/
// Set AP credentials
#define AP_SSID "WSESP8266AP"
#define AP_PASS "password"

// UDP
WiFiUDP UDP;
IPAddress local_IP(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);
#define UDP_PORT 4210

// UDP Buffer
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];



void setup() {
  // put your setup code here, to run once:
  pinMode(D4, OUTPUT);
  //Serial Setup
  Serial.begin(115200);
  Serial.println();
 // Begin Access Point
  Serial.println("Starting access point...");
  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP(AP_SSID, AP_PASS); 
  Serial.println(WiFi.localIP());

  // Begin listening to UDP port
  UDP.begin(UDP_PORT);
  Serial.print("Listening on UDP port ");
  Serial.println(UDP_PORT);
}

void loop() {
  // put your main code here, to run repeatedly:
  // reads 1 byte at a time
  UDP.parsePacket();
  UDP.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);  // << reads here
  if (packetBuffer[0])                             // << test to make sure it is connecting
  {
    digitalWrite(D4, HIGH);
  } else
  {
    digitalWrite(D4, LOW);
  }
    /*
  digitalWrite(D4, HIGH);
  delay(1000);
  digitalWrite(D4, LOW);
  delay(1000);
*/
}
