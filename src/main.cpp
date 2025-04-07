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
#include "builtin_led.h"
#include "globals.h"

// Timer variables
unsigned long previousMillis = 0;
const long updateInterval = 1000; // Update every second

// Function to check button press
void checkButton()
{
  static bool lastButtonState = HIGH;
  static unsigned long lastDebounceTime = 0;
  const unsigned long debounceDelay = 50;

  // Read the current button state
  bool buttonState = digitalRead(BUTTON);

  // Check if button state changed
  if (buttonState != lastButtonState)
  {
    lastDebounceTime = millis();
  }

  // If button state is stable for debounceDelay
  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    // If button was released (assuming active LOW)
    if (buttonState == HIGH && lastButtonState == LOW)
    {
      // Button was pressed and released - toggle LED pattern
      static uint8_t ledPattern = LED_PATTERN_BLINK_SLOW;
      ledPattern = (ledPattern + 1) % 6; // Cycle through patterns
      setBuiltinLEDPattern(ledPattern);

      // Visual feedback with green LED
      digitalWrite(GREEN_LED, LOW);
      delay(50);
      digitalWrite(GREEN_LED, HIGH);
    }
  }

  lastButtonState = buttonState;
}

void setup()
{
  Serial.begin(9600);
  delay(100);
  Serial.println("Cumin Lander Starting...");

  // Configure GPIO pins for LEDs and button
  pinMode(RED_LED, OUTPUT);      // Red LED
  pinMode(GREEN_LED, OUTPUT);    // Green LED
  pinMode(LED_BLUE, OUTPUT);     // Blue LED
  pinMode(LED_BUILTIN, OUTPUT);  // Built-in LED
  pinMode(BUTTON, INPUT_PULLUP); // Button with pull-up

  // Initialize all LEDs to OFF (HIGH for active LOW)
  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(LED_BLUE, HIGH);
  digitalWrite(LED_BUILTIN, HIGH);

  // Initialize modules
  Wire.begin(); // Initialize I2C bus
  initSensors();
  initDisplay();
  initBLE();
  initExternalLED(); // Initialize our external LED on pin D6
  initBuiltinLED();  // Initialize built-in LED patterns

  // Set default time
  setTime(12, 0, 0, 1, 1, 2024);

  // Signal all modules initialized
  digitalWrite(GREEN_LED, LOW);
  delay(500);
  digitalWrite(GREEN_LED, HIGH);

  Serial.println("Initialization complete");
}

void loop()
{
  unsigned long currentMillis = millis();

  // Check for button press to change LED patterns
  checkButton();

  // Update every second
  if (currentMillis - previousMillis >= updateInterval)
  {
    previousMillis = currentMillis;

    // Update BLE (process incoming data)
    updateBLE();

    // Update displays and LEDs
    updateDisplay();     // Update the OLED display
    updateExternalLED(); // Update the external LED based on sensor readings
    updateBuiltinLED();  // Update the built-in LED pattern

    // Blink blue LED to indicate activity
    digitalWrite(LED_BLUE, LOW);
    delay(50);
    digitalWrite(LED_BLUE, HIGH);
  }
}