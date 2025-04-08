#include "sensor_module.h"
#include <Wire.h>
#include "globals.h"

Adafruit_BME280 bme; // Global sensor instance

int tempC;
int humi;
int pressure_hPA;

void initSensors()
{
  unsigned status = bme.begin(0x76); // I2C address for BME280
  if (!status)
  {
    Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
    Serial.print("SensorID was: 0x");
    Serial.println(bme.sensorID(), HEX);
    while (1)
    {
      delay(10);
    }
  }
  Serial.println("BME280 sensor initialized.");
}

void updateSensor()
{
  tempC = bme.readTemperature();
  humi = bme.readHumidity();
  pressure_hPA = bme.readPressure() / 100.0F; // Convert to hPa
}
