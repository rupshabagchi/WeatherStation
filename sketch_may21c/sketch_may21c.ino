#include <SPI.h>
#include <WiFi.h>

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600); 
  Serial.println("hey ");



  Serial.print("Firmware version: ");
  Serial.println(WiFi.firmwareVersion());
}

void loop() {
Serial.print("Firmware version: ");
  Serial.println(WiFi.firmwareVersion());
}
