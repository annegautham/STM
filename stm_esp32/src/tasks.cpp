#include <Arduino.h>
#include "tasks.hpp"
#include "serial.hpp"
#include "server.hpp"

extern TaskHandle_t serialTaskHandle;

void initTasks() {
  xTaskCreatePinnedToCore(serialTask, "SerialTask", 4096, NULL, 1, &serialTaskHandle, 1);
  //xTaskCreatePinnedToCore(serialTask, "Serial", 4096, NULL, 1, NULL, 1);
  Serial.println("RTOS Tasks Started!");
}
