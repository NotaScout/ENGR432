#include <HardwareSerial.h>

#define RX_PIN 3 // GPIO3
#define TX_PIN 1 // GPIO1
#define BAUD_RATE 9600

HardwareSerial Serial2(1); // UART1



void setup() {
  Serial.begin(BAUD_RATE); // Initialize serial monitor for debugging
  Serial1.begin(BAUD_RATE); // Initialize UART1 (Serial1) with default pins GPIO3 (RX) and GPIO1 (TX)
  Serial.println("Reset");
}

void loop() {
  if (Serial1.available()) {
    // Read data from UART1 and print it to the serial monitor
    Serial.print("Received: ");
    while (Serial1.available()) {
      Serial.print((char)Serial1.read());
    }
    Serial.println();
  }
}

