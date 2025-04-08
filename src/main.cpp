#include <Arduino.h>
#include <TimeLib.h>
#include "globals.h"
#include "ble_module.h"
#include "sensor_module.h"
#include "display_module.h"
#include "sound_module.h"
#include "blinking_led.h"
#include "maneki_neko.h"

void setup()
{
  Serial.begin(9600);
  delay(100);

  // Initialize modules
  initBLE();
  initSensors();
  initDisplay();
  initManekiNeko();

  // Set pin modes
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(speakerPin, OUTPUT);

  // Play startup melody
  playStartupMelody();

  // Turn off LEDs (LED_BLUE assumed active low)
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(LED_BLUE, HIGH);

  // Set initial time (example values)
  hourFormat12();
  setTime(19, 27, 36, 17, 1, 2024);

  Serial.println("-- Begin Test --");
}

void loop()
{
  // Process BLE UART input to potentially update the time.
  updateBLE();

  // Update sensor data if needed (readings are taken in updateDisplay).
  updateSensor();

  // Update the OLED display with current time and sensor readings.
  // updateDisplay();
  // switch display mode
  checkDisplayToggle();
  if (showManekiNeko)
  {
    updateManekiNeko(display);
  }
  else
  {
    updateDisplay();
  }

  // Blink LEDs as a visual indicator.
  blinkLEDs();

  if (showManekiNeko)
  {
    delay(100); // Faster animation for bongo cat
  }
  else
  {
    delay(400);
  }
}
