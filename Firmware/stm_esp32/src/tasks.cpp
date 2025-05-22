#include <Arduino.h>
#include "tasks.hpp"
#include "serial.hpp"
#include "server.hpp"

void initTasks() {
  xTaskCreatePinnedToCore(serialTask, "Serial", 4096, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(wsTask,     "WebSock", 4096, NULL, 1, NULL, 0);
}
