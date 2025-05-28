#ifndef SERVER_HPP
#define SERVER_HPP

#include <WebSocketsServer.h>
#include "config.hpp"

// Initializes server, routes, and handlers
void initWebServer();

void initSPIFFS();

// WebSocket task to stream scan lines to browser
void wsTask(void *param);

void listSPIFFS();

#endif
