#ifndef EXTERNAL_LED_H
#define EXTERNAL_LED_H

#include <Arduino.h>

// Initialize the external LED
void initExternalLED();

// Set the external LED state (on/off)
void setExternalLED(bool state);

// Blink the external LED a specified number of times
void blinkExternalLED(int times, int delayMs);

// Update the external LED based on sensor readings
void updateExternalLED();

#endif