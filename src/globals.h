#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>

// Pin definitions
#define RED_LED      D2
#define GREEN_LED    D6
#ifndef LED_BLUE
#define LED_BLUE     D7
#endif      
#define speakerPin   D0
#define BUTTON       D1

// Display settings
#define SCREEN_WIDTH 128    // OLED display width in pixels
#define SCREEN_HEIGHT 32    // OLED display height in pixels
#define OLED_RESET   -1     // Reset pin (-1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C // I2C address for the display

// Sensor
#define SEALEVELPRESSURE_HPA 1013.25

// Tone (melody) definitions
#define NOTE_G3 2551
#define NOTE_G4 1276
#define NOTE_C5 956
#define NOTE_E5 759
#define NOTE_G5 638
#define RELEASE 20
#define BPM 100

// Melody data (12 notes)
extern int melody[];
extern int noteDurations[];

#endif
