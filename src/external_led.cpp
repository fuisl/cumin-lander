#include "external_led.h"
#include "sensor_module.h"
#include "globals.h"

// Pin definition for the external LED
#define EXTERNAL_LED_PIN D6

// Threshold values for sensor readings that will trigger the LED
#define TEMP_HIGH_THRESHOLD 30.0   // Temperature threshold in Celsius
#define HUMIDITY_HIGH_THRESHOLD 80 // Humidity threshold in percent

void initExternalLED()
{
    // Set the pin as output
    pinMode(EXTERNAL_LED_PIN, OUTPUT);

    // Initialize LED to off state
    digitalWrite(EXTERNAL_LED_PIN, LOW);

    // Blink the LED to indicate initialization
    blinkExternalLED(3, 200);

    Serial.println(F("External LED initialized on pin D6"));
}

void setExternalLED(bool state)
{
    digitalWrite(EXTERNAL_LED_PIN, state ? HIGH : LOW);
}

void blinkExternalLED(int times, int delayMs)
{
    for (int i = 0; i < times; i++)
    {
        digitalWrite(EXTERNAL_LED_PIN, HIGH);
        delay(delayMs);
        digitalWrite(EXTERNAL_LED_PIN, LOW);
        delay(delayMs);
    }
}

void updateExternalLED()
{
    // Read sensor values
    float temperature = bme.readTemperature();
    int humidity = bme.readHumidity();

    // Check if readings are valid
    if (isnan(temperature) || isnan(humidity))
    {
        // If readings are invalid, blink the LED quickly to indicate an error
        blinkExternalLED(2, 50);
        return;
    }

    // Logic to determine LED state based on sensor readings
    if (temperature > TEMP_HIGH_THRESHOLD)
    {
        // Temperature is high - turn on the LED
        setExternalLED(true);
    }
    else if (humidity > HUMIDITY_HIGH_THRESHOLD)
    {
        // Humidity is high - blink the LED slowly
        static unsigned long lastBlink = 0;
        static bool ledState = false;

        // Blink every 1 second
        if (millis() - lastBlink >= 1000)
        {
            lastBlink = millis();
            ledState = !ledState;
            setExternalLED(ledState);
        }
    }
    else
    {
        // Normal conditions - LED is off
        setExternalLED(false);
    }
}