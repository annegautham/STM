#pragma once

#include <Arduino.h>

// Function declarations from SerialCommands.cpp
// Replace these with actual functions from your SerialCommands.ino file
#define ESPSerial Serial2
#define console Serial
void checkSerial();
void serialCommand(String str);