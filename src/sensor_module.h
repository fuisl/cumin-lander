#ifndef SENSOR_MODULE_H
#define SENSOR_MODULE_H

#include <Arduino.h>
#include <Adafruit_BME280.h>

extern Adafruit_BME280 bme;

void initSensors();
void updateSensor(); // (Optional: Expand as needed)

#endif
