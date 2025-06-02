#include <Arduino.h>
void setup() {
  Serial.begin(115200);    // USB Serial Monitor
  Serial2.begin(115200);

  Serial.println("Teensy listening on Serial2...");
}

void loop() {
  if (Serial2.available()) {
    String incoming = Serial2.readStringUntil('\n');  // Read until newline (if ESP sends '\n')
    Serial.print("[From ESP32]: ");
    Serial.println(incoming);
  }
}
