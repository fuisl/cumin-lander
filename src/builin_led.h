#ifndef BUILTIN_LED_H
#define BUILTIN_LED_H

#include <Arduino.h>

// Initialize the built-in LED
void initBuiltinLED();

// Update the built-in LED state based on time and conditions
void updateBuiltinLED();

// Set specific blinking pattern for the built-in LED
void setBuiltinLEDPattern(uint8_t pattern);

// Pattern definitions
#define LED_PATTERN_OFF 0
#define LED_PATTERN_ON 1
#define LED_PATTERN_BLINK_SLOW 2
#define LED_PATTERN_BLINK_FAST 3
#define LED_PATTERN_PULSE 4
#define LED_PATTERN_SOS 5

#endif