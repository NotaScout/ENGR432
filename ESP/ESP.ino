#include <SoftwareSerial.h>

SoftwareSerial mySerial(D4, D5); // RX, TX
// D4 RX - PA9
// D5 TX - PA10
// (Send and Receive)

void setup() {
  Serial.begin(115200);
  mySerial.begin(115200);
}

void loop() {
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
  }
  
  delay(20);
}