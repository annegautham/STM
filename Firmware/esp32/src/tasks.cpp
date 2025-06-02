#include <Arduino.h>
#include "tasks.hpp"
#include "serial.hpp"
#include "server.hpp"

extern TaskHandle_t serialTaskHandle;

void initTasks() {
  xTaskCreatePinnedToCore(serialTask, "SerialTask", 4096, NULL, 1, &serialTaskHandle, 1); //Serial2 recieving messages from Teensy (incoming data)
  Serial.println("RTOS Tasks Started!");
}
