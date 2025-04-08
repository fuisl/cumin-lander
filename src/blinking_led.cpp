#include "blinking_led.h"

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
        digitalWrite(LED_RED, LOW);
        delay(10);
        digitalWrite(LED_RED, HIGH);
        delay(50);
    }
}