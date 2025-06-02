#include <Arduino.h>
// For ESP32, Serial is usually connected to the USB/PC
// Serial2 can be used for other devices (e.g., Teensy)

void setup() {
  // Initialize Serial (USB)
  Serial.begin(115200);
  while (!Serial);  // Wait for Serial to be ready

  // Optional: Initialize Serial2 (UART2)
  // TX2 (GPIO17), RX2 (GPIO16) are defaults, but can be redefined
  Serial2.begin(115200, SERIAL_8N1, 16, 17); // RX=16, TX=17
}

void loop() {
  // Send a message to Serial (USB)
  Serial.println("Hello from ESP32 via Serial!");

  // Optional: Send a message to Serial2 (UART2)
  Serial2.println("Hello from ESP32 via Serial2!");

  // Wait 1 second
  delay(1000);
}