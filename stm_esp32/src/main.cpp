#include <Arduino.h>
#include <SPIFFS.h>
#include <WebServer.h>

#include "config.hpp"
#include "wifi.hpp"
#include "server.hpp"
#include "serial.hpp"
#include "tasks.hpp"

extern WebSocketsServer ws;
extern WebServer server;

void setup() {
  Serial.begin(115200); //Serial0 for interface
  Serial2.begin(UART_BAUDRATE, SERIAL_8N1, UART_RX, UART_TX); //Serial2 for Teensy UART

  //initWiFi(); 
  connect_wifi_enterprise(); //eduroam works!
  initSPIFFS(); 
  initWebServer();
  initTasks();
  Serial.println("Setup complete!");
}

void loop() {
  //all freertos tasks
   ws.loop(); //web server not synchronous
   ws.onEvent(onWsEvent);
}

