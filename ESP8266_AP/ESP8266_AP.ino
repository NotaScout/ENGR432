#include <ESP8266WiFi.h>
#include <WiFiUdp.h>


void setup() {
  // put your setup code here, to run once:
  pinMode(D4, OUTPUT);
/*
We will define the constant AD_SSID, which will contain the name of our ESP8266 router.
*/
// Set AP credentials
#define AP_SSID "TheOtherESP"
#define AP_PASS "flashmeifyoucan"


}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(D4, HIGH);
  delay(1000);
  digitalWrite(D4, LOW);
  delay(1000);
}
