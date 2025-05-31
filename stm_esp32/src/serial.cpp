#include <Arduino.h>

#include "config.hpp"
#include "serial.hpp"

#define NUM_PIXELS 512
#define NUM_CHANNELS 4
#define DATA_SIZE (NUM_PIXELS * NUM_CHANNELS * sizeof(int32_t))
#define PACKET_SIZE (2 + 2 + DATA_SIZE + 2)  // Header + Line + Data + CRC

// Internal buffer
static uint8_t packetBuffer[PACKET_SIZE];
static int bufferPos = 0;
static bool readingPacket = false;

// RTOS task handle
TaskHandle_t serialTaskHandle = NULL;

// Forward declarations
void serialTask(void *param);
void processPacket(uint8_t *data);
int32_t bytesToInt32(uint8_t *b);
uint16_t crc16_update(uint16_t crc, uint8_t a);

// Serial Task: Poll Serial2 for incoming data
void serialTask(void *param) {
  Serial2.begin(115200, SERIAL_8N1, 16, 17); // Adjust pins as needed

  Serial.println("[SerialTask] Ready to receive STM packets...");

  while (true) {
    while (Serial2.available()) {
      uint8_t byte = Serial2.read();

      static uint8_t lastByte = 0;
      if (!readingPacket) {
        if (lastByte == 0xAA && byte == 0x55) {
          readingPacket = true;
          bufferPos = 0;
        }
        lastByte = byte;
      } else {
        packetBuffer[bufferPos++] = byte;
        if (bufferPos >= PACKET_SIZE - 2) {  // Exclude header
          processPacket(packetBuffer);
          readingPacket = false;
        }
      }
    }
    vTaskDelay(pdMS_TO_TICKS(1));  // Yield to other tasks
  }
}

// Process a single packet: validate CRC, print debug info
void processPacket(uint8_t *data) {
  uint16_t lineNum = (data[0] << 8) | data[1];

  // CRC calculation
  uint16_t crc_calc = 0xFFFF;
  crc_calc = crc16_update(crc_calc, data[0]);
  crc_calc = crc16_update(crc_calc, data[1]);
  for (int i = 0; i < DATA_SIZE; i++) {
    crc_calc = crc16_update(crc_calc, data[2 + i]);
  }
  uint16_t crc_recv = (data[2 + DATA_SIZE] << 8) | data[2 + DATA_SIZE + 1];

  Serial.print("\n[Line ");
  Serial.print(lineNum);
  Serial.println("]");

  if (crc_calc != crc_recv) {
    Serial.println("CRC MISMATCH! Packet discarded.");
    Serial.print("Expected: 0x"); Serial.print(crc_calc, HEX);
    Serial.print(" | Received: 0x"); Serial.println(crc_recv, HEX);
    return;
  }

  Serial.println("CRC OK.");

  // Print a few pixels
  for (int i = 0; i < NUM_PIXELS; i += 100) {
    int offset = i * NUM_CHANNELS * 4;
    int32_t z = bytesToInt32(&data[2 + offset]);
    int32_t zr = bytesToInt32(&data[2 + offset + 4]);
    int32_t e = bytesToInt32(&data[2 + offset + 8]);
    int32_t er = bytesToInt32(&data[2 + offset + 12]);

    Serial.print("Pixel "); Serial.print(i);
    Serial.print(": z="); Serial.print(z);
    Serial.print(", zr="); Serial.print(zr);
    Serial.print(", e="); Serial.print(e);
    Serial.print(", er="); Serial.println(er);
  }

  Serial.print("CRC: 0x"); Serial.println(crc_recv, HEX);
  Serial.println("----");
}

int32_t bytesToInt32(uint8_t *b) {
  return ((int32_t)b[0] << 24) | ((int32_t)b[1] << 16) |
         ((int32_t)b[2] << 8) | (int32_t)b[3];
}

uint16_t crc16_update(uint16_t crc, uint8_t a) {
  crc ^= a;
  for (int i = 0; i < 8; ++i) {
    if (crc & 1) crc = (crc >> 1) ^ 0x8408;
    else crc = crc >> 1;
  }
  return crc;
}
