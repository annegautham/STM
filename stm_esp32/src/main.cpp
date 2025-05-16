#include <Arduino.h>
#include <WiFi.h>
#include "esp_wpa2.h"
#include <WebSocketsClient.h>
#include "secrets.h"


/* ===== address of the PC running Tornado ===== */
const char *pc_host  = "165.124.85.137";   // or "pc.local" if mDNS works
const uint16_t pc_port = 8888;           // same port as Tornado
const char *ws_path  = "/ws";            // Tornado endpoint

#define RXD2 16
#define TXD2 17


/* ---- WebSocket client instance ---- */
WebSocketsClient ws;

HardwareSerial& espSerial = Serial2; //Lets us UART2

// RTOS queue for thread-safe data handling
QueueHandle_t uartQueue;

// Task1: UART receiver
void uartTask(void* pvParameters) {
  espSerial.begin(115200, SERIAL_8N1, RXD2, TXD2);
  String incomingData = "";

  for (;;) {
    while (espSerial.available()) {
      char c = espSerial.read();
      if (c == '\n') { //new line means end of packet
        if (incomingData.length() > 0) {
          char* data = strdup(incomingData.c_str());
          if (xQueueSend(uartQueue, &data, portMAX_DELAY) != pdPASS) {
            free(data);  // Free memory if not sent
          }
          incomingData = ""; //clear buffer
        }
      } else {
        incomingData += c; //append char to buffer if its not new line
      }
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);  // Yield CPU briefly
  }
}

// Task2: WebSocket sender
void wsTask(void* pvParameters) {
  ws.begin(pc_host, pc_port, ws_path);
  ws.setReconnectInterval(2000);

  char* dataToSend = NULL;

  for (;;) {
    ws.loop();

    if (xQueueReceive(uartQueue, &dataToSend, pdMS_TO_TICKS(50)) == pdPASS) {
      Serial.printf("Sending to WebSocket: %s\n", dataToSend);
      ws.sendTXT(dataToSend);
      free(dataToSend);  // Free memory after sending
    }

    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA); 
  WiFi.begin(EAP_WIFI_SSID, WPA2_AUTH_PEAP, EAP_USERNAME, EAP_USERNAME, EAP_PASSWORD);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
  Serial.println("\nConnected! IP: " + WiFi.localIP().toString());

  // Create queue with space for 10 pointers
  uartQueue = xQueueCreate(10, sizeof(char*));

  // Launch RTOS tasks
  xTaskCreatePinnedToCore(uartTask, "UART Receiver", 4096, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(wsTask, "WebSocket Sender", 8192, NULL, 1, NULL, 1);
}

void loop() {
  vTaskDelay(portMAX_DELAY);
}