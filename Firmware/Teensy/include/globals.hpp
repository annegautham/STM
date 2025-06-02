#pragma once
#include <Arduino.h>

// Flags and states
extern bool serialEnabled;
extern bool scanningEnabled;
extern int scanSize;
extern unsigned int pixelsPerLine;
extern volatile unsigned int pixelCounter;
extern float lineRate;
extern int Kp, Ki;
extern int dt;
extern int bias, setpoint, setpointLog;
extern volatile int x, y;
extern int xo, yo;
extern const int logTable[];

// Pins
#define SERIAL_LED 13  // Replace 13 with actual pin
#define DAC_CH_BIAS 0     // Replace with actual channel

// Functions
void moveTip(int x, int y);
void resetScan();
void updateStepSizes();
void retract();
extern bool engage();
extern bool manualEnabled;
extern void stepMotors(int steps, int dir, int timePerStep);


#include <DAC8814.h>  // or the correct header for your DAC

extern DAC8814 dac;