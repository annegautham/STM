#include <Arduino.h>
#include <WiFi.h>

#include "wifi.hpp"
#include "esp_wpa2.h"

const char* WIFI_SSID = "Device-Northwestern";

void initWiFi() {
    WiFi.mode(WIFI_STA);
    Serial.println("Connecting to WiFi...");
    WiFi.begin(WIFI_SSID);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
  Serial.println("\nConnected! IP: " + WiFi.localIP().toString());
}
