#include <Arduino.h>

#include "config.hpp"
#include "serial.hpp"

QueueHandle_t lineQ = NULL;

uint16_t crc16_kermit(const uint8_t *data, size_t len) {
  uint16_t crc = 0;
  for (size_t i = 0; i < len; ++i) {
    crc ^= data[i];
    for (uint8_t j = 0; j < 8; ++j)
      crc = (crc >> 1) ^ (crc & 1 ? 0x8408 : 0);
  }
  return crc;
}

void initSerial(void){
    lineQ = xQueueCreate(QUEUE_DEPTH, sizeof(ScanLine*));  // assign to global
    if (!lineQ) {
        Serial.println("Failed to create lineQ");
        return;
    }
    else{
        Serial.println("Scan Queue Created...");
    }
    return;
}

void serialTask(void *param) {
  const int HEADER = 8, PAYLOAD = PIXELS * 4, CRC = 2;
  uint8_t buf[HEADER + PAYLOAD + CRC];

  while (true) {
    if (Serial2.available() >= 2 && Serial2.peek() == 0xA5) {
      Serial2.read(); if (Serial2.read() != 0x5A) continue;

      Serial2.readBytes(&buf[2], HEADER - 2);
      uint16_t len = buf[6] | (buf[7] << 8);
      if (len != PAYLOAD) continue;
      Serial2.readBytes(&buf[8], PAYLOAD + CRC);

      uint16_t calc = crc16_kermit(buf, HEADER + PAYLOAD);
      uint16_t recv = buf[8 + PAYLOAD] | (buf[8 + PAYLOAD + 1] << 8);

      if (calc == recv) {
        ScanLine *line = (ScanLine *)heap_caps_malloc(sizeof(ScanLine), MALLOC_CAP_8BIT);
        for (int i = 0; i < PIXELS; ++i) {
          line->z[i]   = buf[8 + i*4 + 0] | (buf[8 + i*4 + 1] << 8);
          line->err[i] = buf[8 + i*4 + 2] | (buf[8 + i*4 + 3] << 8);
        }
        xQueueSend(lineQ, &line, 0);
      }
    }
    vTaskDelay(1);
  }
}
