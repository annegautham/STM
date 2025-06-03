#ifndef SERVER_HPP
#define SERVER_HPP

#include <WebSocketsServer.h>
#include "config.hpp"

// Initializes server, routes, and handlers
void initWebServer();
void initSPIFFS();
void listSPIFFS();
void onWsEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);

#endif
