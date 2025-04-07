#include <Arduino.h>
#include <TimeLib.h>
#include "globals.h"
#include "ble_module.h"
#include "sensor_module.h"
#include "display_module.h"
#include "sound_module.h"

// Helper function: Blink LEDs
void blinkLEDs()
{
  for (int i = 0; i < 5; i++)
  {
    digitalWrite(GREEN_LED, HIGH);
    delay(10);
    digitalWrite(GREEN_LED, LOW);
    delay(50);
  }
  for (int i = 0; i < 5; i++)
  {
    digitalWrite(RED_LED, HIGH);
    delay(10);
    digitalWrite(RED_LED, LOW);
    delay(50);
  }
}

void setup()
{
  Serial.begin(9600);
  delay(100);

  // Initialize modules
  initBLE();
  initSensors();
  initDisplay();

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

// Thêm biến để lưu trạng thái nhấp nháy
unsigned long ledPreviousMillis = 0;
const long ledBlinkInterval = 100;             // Thời gian nhấp nháy (ms)
const int ledPatterns[] = {100, 50, 100, 700}; // Mẫu nhấp nháy (ms): sáng, tắt, sáng, tắt
int ledPatternIndex = 0;
bool ledState = false;

// Thêm hàm này vào phần trước void loop()
void updateLEDEffect()
{
  unsigned long currentMillis = millis();

  if (currentMillis - ledPreviousMillis >= ledPatterns[ledPatternIndex])
  {
    ledPreviousMillis = currentMillis;

    // Đảo trạng thái LED
    ledState = !ledState;

    // Sáng thì LOW, tắt thì HIGH (active LOW)
    digitalWrite(LED_BUILTIN, ledState ? LOW : HIGH);

    // Chuyển sang mẫu tiếp theo
    ledPatternIndex = (ledPatternIndex + 1) % (sizeof(ledPatterns) / sizeof(ledPatterns[0]));
  }
}

void loop()
{
  // Process BLE UART input to potentially update the time.
  updateBLE();

  // Update sensor data if needed (readings are taken in updateDisplay).
  updateSensor();

  // Update the OLED display with current time and sensor readings.
  updateDisplay();

  // Blink LEDs as a visual indicator.
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= updateInterval)
  {
    previousMillis = currentMillis;
    digitalWrite(BUILTIN_LED, LOW);
    delay(50);
    digitalWrite(BUILTIN_LED, HIGH);
  }

  delay(400);
}
