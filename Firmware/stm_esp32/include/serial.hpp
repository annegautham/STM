#ifndef SERIAL_HPP
#define SERIAL_HPP

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

#include "config.hpp"

// Struct representing one scan line
struct ScanLine {
  uint16_t z[PIXELS];
  uint16_t err[PIXELS];
};

// Global FreeRTOS queue for scan lines
extern QueueHandle_t lineQ;

void initSerial(void);

// Reads UART, parses scan lines, validates CRC
void serialTask(void *param);

// CRC-16 (Kermit) function used for packet integrity
uint16_t crc16_kermit(const uint8_t *data, size_t len);

#endif
