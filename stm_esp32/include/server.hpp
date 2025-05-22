#ifndef SERVER_HPP
#define SERVER_HPP

#include <ESPAsyncWebServer.h>

#include "config.hpp"

// Initializes server, routes, and handlers
void initWebServer();

void initSPIFFS();

// WebSocket → Teensy command relay
void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
               AwsEventType type, void *arg, uint8_t *data, size_t len);

// WebSocket task to stream scan lines to browser
void wsTask(void *param);

#endif
