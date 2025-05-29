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

// #define EAP_ANONYMOUS_IDENTITY "anonymous@northwestern.edu" //anonymous@example.com, or you can use also nickname@example.com
// #define EAP_IDENTITY "vcs5888@u.northwestern.edu" //nickname@example.com, at some organizations should work nickname only without realm, but it is not recommended
// #define EAP_PASSWORD "dcm93130704Nw!!!" //password for eduroam account
// #define EAP_USERNAME "vcs5888@u.northwestern.edu" // the Username is the same as the Identity in most eduroam networks.

// //SSID NAME
// const char* ssid = "eduroam"; // eduroam SSID


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