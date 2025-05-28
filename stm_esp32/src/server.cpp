#include <Arduino.h>
#include "server.hpp"
#include "serial.hpp"
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <WebServer.h>
#include <WiFi.h>
#include <WebSocketsServer.h>
#include <SPIFFS.h>

// Global WebSocket server on port 81
WebSocketsServer ws(81);
WebServer server(80);  // HTTP server on port 80

// Forward declarations
void handleClient();

// WebSocket task: stream ScanLine objects to clients
void wsTask(void *param) {
  ScanLine *line;
  while (true) {
    if (xQueueReceive(lineQ, &line, portMAX_DELAY)) {
      ws.broadcastBIN((uint8_t *)line, sizeof(ScanLine));
      heap_caps_free(line);
    }
  }
}

// Serve index.html from SPIFFS
void handleRoot() {
  File file = SPIFFS.open("/index.html", "r");
  if (!file) {
    Serial.println("index.html not found in SPIFFS");
    server.send(404, "text/plain", "File not found");
    return;
  }
  server.streamFile(file, "text/html");
  file.close();
}

// Initialize SPIFFS
void initSPIFFS() {
  if (!SPIFFS.begin(true)) {  // Auto-format on first boot
    Serial.println("Failed to mount or format SPIFFS");
    while (true) { delay(1000); }
  } else {
    listSPIFFS();
    Serial.println("SPIFFS mounted successfully");
  }
}

// List SPIFFS files (for debug)
void listSPIFFS() {
  File root = SPIFFS.open("/");
  File file = root.openNextFile();
  while (file) {
    Serial.print("  📄 ");
    Serial.print(file.name());
    Serial.print("\t");
    Serial.println(file.size());
    file = root.openNextFile();
  }
}

// Initialize web server and websocket
void initWebServer() {
  // HTTP route: index.html
  server.on("/", handleRoot);
  server.onNotFound([]() {
    server.send(404, "text/plain", "Not found");
  });

  server.begin();
  Serial.println("HTTP server started on port 80");

  ws.begin();
  Serial.println("WebSocket server started on port 81");

  // Web server task (poll HTTP and WebSocket in the background)
  xTaskCreate(
    [](void*) {
      while (true) {
        server.handleClient();  // Handle HTTP clients
        ws.loop();              // Handle WebSocket events
        vTaskDelay(10 / portTICK_PERIOD_MS);
      }
    },
    "WebServerTask", 4096, NULL, 1, NULL
  );
}