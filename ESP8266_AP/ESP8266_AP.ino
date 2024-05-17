#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

// Set AP credentials
#define AP_SSID "WSESP8266AP"
#define AP_PASS "password"
// UDP
WiFiUDP UDP;
IPAddress local_IP(192,168,4,1);
IPAddress gateway(192,168,4,1);
IPAddress subnet(255,255,255,0);
#define UDP_PORT 4210

// UDP Buffer
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];

void setup() {
  // Setup LED pin
  pinMode(2, OUTPUT);
  
  // Setup serial port
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
  int packetSize = UDP.parsePacket();
  if (packetSize) {
    // Clear the buffer
    memset(packetBuffer, 0, UDP_TX_PACKET_MAX_SIZE);

    // Read the packet into the buffer
    int len = UDP.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE - 1);
    if (len > 0) {
      packetBuffer[len] = 0; // Null-terminate the string
    }

    // Print packet size and contents as hexadecimal values
    Serial.print("Received packet size: ");
    Serial.println(len);
    Serial.print("Received packet: ");
    for (int i = 0; i < len; i++) {
      Serial.print("0x");
      if (packetBuffer[i] < 0x10) {
        Serial.print("0");
      }
      Serial.print(packetBuffer[i], HEX);
      Serial.print(" ");
    }
    Serial.println();

    // Concatenate packet bytes into a single value
    if (len >= 2) {
      uint16_t combinedValue = (static_cast<uint16_t>(packetBuffer[1]) << 8) | static_cast<uint8_t>(packetBuffer[0]);
      Serial.print("Concatenated value: ");
      Serial.println(combinedValue, DEC);
      
      Serial.println(combinedValue/10, DEC);
      Serial.print(" Degrees ");
    
    }

    // Check the first byte of the packet to control the LED
    if (packetBuffer[0]) {
      digitalWrite(2, HIGH);
    } else {
      digitalWrite(2, LOW);
    }
  }
  delay(20);
}
