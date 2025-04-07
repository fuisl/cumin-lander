#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BME280.h>
#include <TimeLib.h>

// Include our modules
#include "display_module.h"
#include "sensor_module.h"
#include "ble_module.h"
#include "external_led.h"
#include "globals.h"

// Timer variables
unsigned long previousMillis = 0;
const long updateInterval = 1000; // Update every second

void setup()
{
  Serial.begin(9600);
  delay(100);
  Serial.println("Cumin Lander Starting...");

  // Configure GPIO pins for LEDs
  pinMode(RED_LED, OUTPUT);   // Red LED
  pinMode(GREEN_LED, OUTPUT); // Green LED
  pinMode(LED_BLUE, OUTPUT);  // Blue LED

  // Initialize all LEDs to OFF (HIGH for active LOW)
  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(LED_BLUE, HIGH);

  // Initialize modules
  Wire.begin(); // Initialize I2C bus
  initSensors();
  initDisplay();
  initBLE();
  initExternalLED(); // Initialize our external LED on pin D6

  // Set default time
  setTime(12, 0, 0, 1, 1, 2024);

  Serial.println("Initialization complete");
}

void loop()
{
  unsigned long currentMillis = millis();

  // Update every second
  if (currentMillis - previousMillis >= updateInterval)
  {
    previousMillis = currentMillis;

    // Update BLE (process incoming data)
    updateBLE();

    // Update displays and external LED
    updateDisplay();     // Update the OLED display
    updateExternalLED(); // Update the external LED based on sensor readings

    // Blink blue LED to indicate activity
    digitalWrite(LED_BLUE, LOW);
    delay(50);
    digitalWrite(LED_BLUE, HIGH);
  }
}