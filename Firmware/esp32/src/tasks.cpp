#include <Arduino.h>
#include "tasks.hpp"
#include "serial.hpp"
#include "server.hpp"

extern TaskHandle_t serialTaskHandle;
QueueHandle_t teensyQueue;  

void initTasks() {
  teensyQueue = xQueueCreate(10, sizeof(String));
  //xTaskCreatePinnedToCore(serialTask, "SerialTask", 4096, NULL, 1, &serialTaskHandle, 1); //Serial2 recieving messages from Teensy (incoming data)
  xTaskCreatePinnedToCore(teensySenderTask, "TeensySender", 4096, NULL, 1, NULL, 1);
  Serial.println("RTOS Tasks Started!");
}


void teensySenderTask(void* param) {
  String msg;
  for (;;) {
    if (xQueueReceive(teensyQueue, &msg, portMAX_DELAY) == pdTRUE) {
      Serial2.print(msg);  // write in a clean, isolated context
    }
  }
}
