#include <Arduino.h>
#include <WiFi.h>

#include "secrets.h"
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

// void initWiFi() {
//     Serial.print(F("Connecting to network: "));
//     Serial.println(ssid);
//     // WPA2 enterprise magic starts here
//     WiFi.disconnect(true);     
//     WiFi.begin(ssid, WPA2_AUTH_PEAP, EAP_IDENTITY, EAP_USERNAME, EAP_PASSWORD);
//     // WPA2 enterprise magic ends here
//     while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(F("."));
//   }
//     Serial.println("");
//     Serial.println(F("WiFi is connected!"));
//     Serial.println(F("IP address set: "));
//     Serial.println(WiFi.localIP()); //print LAN IP

// }

void connect_wifi_enterprise(){
    WiFi.disconnect(true);
    int counter = 0;
    WiFi.mode(WIFI_STA);    //init wifi mode
    WiFi.begin(EAP_WIFI_SSID, WPA2_AUTH_PEAP, EAP_USERNAME, EAP_USERNAME, EAP_PASSWORD);

    while(WiFi.status()!= WL_CONNECTED){
        delay(500);
        Serial.print(".");
        counter++;
        if (counter >= 60) {  //after 30 seconds timeout - reset board
        ESP.restart();
        }
    }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address set: ");
  Serial.println(WiFi.localIP());  //print LAN IP}
}

