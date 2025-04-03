#include "sensor_module.h"
#include <Wire.h>

Adafruit_BME280 bme; // Global sensor instance

void initSensors() {
  unsigned status = bme.begin();
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
    Serial.print("SensorID was: 0x");
    Serial.println(bme.sensorID(), HEX);
    while (1) {
      delay(10);
    }
  }
  Serial.println("BME280 sensor initialized.");
}

void updateSensor() {
  // For now this can be a placeholder.
  // Sensor readings are performed directly in display_module.cpp.
}
