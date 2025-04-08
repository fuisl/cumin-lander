#ifndef MANEKI_NEKO_MODULE_H
#define MANEKI_NEKO_MODULE_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Animation state
extern bool showManekiNeko;
extern uint8_t animationFrame;
extern unsigned long lastFrameTime;
extern unsigned long lastDisplayToggleTime;
extern const unsigned long DISPLAY_TOGGLE_INTERVAL;

// Define animation constants
#define ANIM_SIZE 128  // 32*32/8 = 128 bytes per frame
#define FRAME_COUNT 28 // Number of frames in the animation

// Initialize bongo cat animation
void initManekiNeko();

// Draw the current bongo cat animation frame
void updateManekiNeko(Adafruit_SSD1306 &display);

// Check if it's time to switch displays and toggle if needed
void checkDisplayToggle();

#endif // MANEKI_NEKO_MODULE_H