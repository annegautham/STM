#include <Arduino.h>
#include "server.hpp"
#include "serial.hpp"
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

AsyncWebServer server(80); //creaste asynchronous webserver on port 80 (http)
AsyncWebSocket ws("/ws"); //create websocket endpoint at /ws

extern QueueHandle_t lineQ; //tells compiler that FreeRTOS queue declared in another file
                            // used to receive pointers to ScanLine structs.

void onWsEvent(AsyncWebSocket *s, AsyncWebSocketClient *c, AwsEventType t,
               void *arg, uint8_t *data, size_t len) {
  if (t != WS_EVT_DATA) return; //exit if it isnt an event 
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (!info->final || info->opcode != WS_TEXT) return;

  StaticJsonDocument<128> doc; //Parses incoming JSON text into doc
  deserializeJson(doc, data, len); 
  const char* cmd = doc["cmd"]; //extract json values 
  int val = doc["value"] | 0;

  char buf[32]; 
  if (val) snprintf(buf, sizeof(buf), "%s%d\n", cmd, val);
  else     snprintf(buf, sizeof(buf), "%s\n", cmd);
  Serial2.print(buf);
}

void initWebServer() {
  ws.onEvent(onWsEvent);
  server.addHandler(&ws);

  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

  server.on("/api/status", HTTP_GET, [](AsyncWebServerRequest *req) {
    String json = "{\"heap\":" + String(ESP.getFreeHeap()) + "}";
    req->send(200, "application/json", json);
  });

  server.begin();
}

void wsTask(void *param) {
  ScanLine *line;
  while (true) {
    if (xQueueReceive(lineQ, &line, portMAX_DELAY)) {
      ws.binaryAll((uint8_t *)line, sizeof(ScanLine));
      heap_caps_free(line);
    }
  }
}

void initSPIFFS(){
    if (!SPIFFS.begin()) {
    Serial.println("Failed to mount SPIFFS");
    return;
    }
}

