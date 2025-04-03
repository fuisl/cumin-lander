#ifndef BLE_MODULE_H
#define BLE_MODULE_H

#include <Arduino.h>
#include <TimeLib.h>

// Initialize BLE and start advertising.
void initBLE();

// Process incoming BLE data (for example, updating the system time).
void updateBLE();

#endif
