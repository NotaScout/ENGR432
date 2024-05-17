/* RECEIVER ESP8266 */



// Set baud and switch Rx/Tx to D7/D8
void setup() {
  Serial.begin(115200);
  Serial2.begin(115200,SERIAL_8N1, 16, 17);
}

void loop() {
  Serial.println("Message: ");
  Serial.println(Serial2.readString());
}

