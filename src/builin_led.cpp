#include "builtin_led.h"
#include "globals.h"
#include <TimeLib.h>

// Current LED pattern
static uint8_t currentPattern = LED_PATTERN_OFF;

// Variables for pattern timing
static unsigned long lastPatternUpdate = 0;
static uint8_t patternStep = 0;

// SOS pattern: . . . - - - . . .
const uint8_t sosPattern[] = {1, 0, 1, 0, 1, 0, 2, 0, 2, 0, 2, 0, 1, 0, 1, 0, 1, 0, 3};
const uint8_t sosPatternLength = 19;

void initBuiltinLED()
{
    // The built-in LED is already initialized in main.cpp
    // but we'll add a startup sequence here

    // Flash the LED in a distinct pattern to indicate startup
    for (int i = 0; i < 3; i++)
    {
        digitalWrite(LED_BUILTIN, LOW); // On (active LOW)
        delay(50);
        digitalWrite(LED_BUILTIN, HIGH); // Off
        delay(50);
    }

    delay(200);

    for (int i = 0; i < 2; i++)
    {
        digitalWrite(LED_BUILTIN, LOW); // On
        delay(200);
        digitalWrite(LED_BUILTIN, HIGH); // Off
        delay(200);
    }

    // Set the initial pattern
    setBuiltinLEDPattern(LED_PATTERN_BLINK_SLOW);

    Serial.println(F("Built-in LED initialized"));
}

void setBuiltinLEDPattern(uint8_t pattern)
{
    currentPattern = pattern;
    patternStep = 0;
    lastPatternUpdate = millis();

    // Immediately apply the first step of the pattern
    switch (pattern)
    {
    case LED_PATTERN_OFF:
        digitalWrite(LED_BUILTIN, HIGH); // Off (active LOW)
        break;
    case LED_PATTERN_ON:
        digitalWrite(LED_BUILTIN, LOW); // On
        break;
    default:
        // For other patterns, will be handled in updateBuiltinLED
        break;
    }
}

void updateBuiltinLED()
{
    unsigned long currentMillis = millis();

    // Change the pattern based on time of day
    time_t t = now();
    int currentHour = hour(t);
    int currentSecond = second(t);

    // Every full minute, do a quick flash (at 0 seconds)
    if (currentSecond == 0)
    {
        static bool minuteFlashDone = false;
        if (!minuteFlashDone)
        {
            digitalWrite(LED_BUILTIN, LOW); // On
            delay(100);
            digitalWrite(LED_BUILTIN, HIGH); // Off
            minuteFlashDone = true;
        }
    }
    else
    {
        // Reset the flag for the next minute
        minuteFlashDone = false;
    }

    // At night (8 PM to 6 AM), use slow breathing pattern
    if (currentHour >= 20 || currentHour < 6)
    {
        if (currentPattern != LED_PATTERN_PULSE)
        {
            setBuiltinLEDPattern(LED_PATTERN_PULSE);
        }
    }
    // During day, use normal blinking pattern
    else
    {
        if (currentPattern != LED_PATTERN_BLINK_SLOW)
        {
            setBuiltinLEDPattern(LED_PATTERN_BLINK_SLOW);
        }
    }

    // Handle different patterns
    switch (currentPattern)
    {
    case LED_PATTERN_OFF:
        digitalWrite(LED_BUILTIN, HIGH); // Off
        break;

    case LED_PATTERN_ON:
        digitalWrite(LED_BUILTIN, LOW); // On
        break;

    case LED_PATTERN_BLINK_SLOW:
        // Blink every 2 seconds
        if (currentMillis - lastPatternUpdate >= 2000)
        {
            lastPatternUpdate = currentMillis;
            digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        }
        break;

    case LED_PATTERN_BLINK_FAST:
        // Blink every 200ms
        if (currentMillis - lastPatternUpdate >= 200)
        {
            lastPatternUpdate = currentMillis;
            digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        }
        break;

    case LED_PATTERN_PULSE:
        // Soft "breathing" effect using PWM
        static const int breatheCycle = 5000; // 5 second full cycle
        int timePassed = (int)(currentMillis % breatheCycle);

        // Calculate brightness using a sine wave
        float normalizedTime = (float)timePassed / breatheCycle;
        float radians = normalizedTime * 2 * PI;
        int brightness = (int)(128 + 127 * sin(radians));

        // Map brightness to PWM range (0-255), but invert for active LOW
        analogWrite(LED_BUILTIN, 255 - brightness);

        // Update less frequently to save CPU
        delay(20);
        break;

    case LED_PATTERN_SOS:
        // SOS pattern (... --- ...)
        if (currentMillis - lastPatternUpdate >= 200)
        {
            lastPatternUpdate = currentMillis;

            // Get the current pattern element
            uint8_t patternValue = sosPattern[patternStep];

            if (patternValue == 0)
            {
                // Gap (off)
                digitalWrite(LED_BUILTIN, HIGH);
            }
            else if (patternValue == 1)
            {
                // Dot (short on)
                digitalWrite(LED_BUILTIN, LOW);
            }
            else if (patternValue == 2)
            {
                // Dash (long on)
                digitalWrite(LED_BUILTIN, LOW);
            }
            else
            {
                // Pause between repetitions
                digitalWrite(LED_BUILTIN, HIGH);
                delay(1000);
            }

            // Move to next step in pattern
            patternStep = (patternStep + 1) % sosPatternLength;
        }
        break;
    }
}