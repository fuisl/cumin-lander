#include "display_module.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <TimeLib.h>
#include "DSEG7_Classic_Mini_Regular_15.h"
#include "sensor_module.h"
#include "globals.h"

// Create display instance
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
GFXcanvas1 canvas(128, 32);

static char timeHour[8];
static char timeMinute[8];
static char timeSecond[8];

// Helper: Convert 24-hour format to 12-hour format.
int timeto12(int hourin24) {
  if (hourin24 == 0)
    return 12;
  if (hourin24 > 12)
    return hourin24 - 12;
  return hourin24;
}

void initDisplay() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (1);
  }
  display.clearDisplay();
  display.display();
}

void updateDisplay() {
  canvas.fillScreen(0);
  canvas.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  canvas.setRotation(1);
  canvas.drawLine(0, 10, 32, 10, SSD1306_WHITE);
  canvas.drawLine(0, 84, 32, 84, SSD1306_WHITE);
  
  time_t t = now();
  
  // Read sensor values
  int tempC         = bme.readTemperature();
  int humi          = bme.readHumidity();
  int pressure_hPa  = bme.readPressure() / 100.0F;
  int altitude_m    = bme.readAltitude(SEALEVELPRESSURE_HPA);
  
  sprintf(timeHour, "%02d", timeto12(hour(t)));
  sprintf(timeMinute, "%02d", minute(t));
  sprintf(timeSecond, "%02d", second(t));
  
  canvas.setFont();
  canvas.setTextSize(1);
  canvas.setCursor(0, 0);
  canvas.print("/////");
  canvas.setCursor(0, 15);
  canvas.print(isPM() ? F("   PM") : F("   AM"));
  
  canvas.setFont(&DSEG7_Classic_Mini_Regular_15);
  canvas.setCursor(5, 40);
  canvas.print(timeHour);
  canvas.setCursor(5, 60);
  canvas.print(timeMinute);
  canvas.setCursor(5, 80);
  canvas.print(timeSecond);
  
  canvas.setFont();
  canvas.setTextSize(1);
  if (!isnan(tempC)) {
    Serial.print("Temp *C = ");
    Serial.print(tempC);
    Serial.print("\t\t");
    canvas.setCursor(0, 90);
    canvas.print("  ");
    canvas.print(tempC);
    canvas.print("C");
  } else {
    Serial.println("Failed to read temperature");
  }
  
  if (!isnan(humi)) {
    Serial.print("Hum. % = ");
    Serial.println(humi);
    canvas.setCursor(0, 105);
    canvas.print("  ");
    canvas.print(humi);
    canvas.print("%");
    canvas.setCursor(0, 120);
    canvas.print(pressure_hPa);
    canvas.print("hPa");
  } else {
    Serial.println("Failed to read humidity");
  }
  
  display.drawBitmap(0, 0, canvas.getBuffer(), 128, 32, 1, 0);
  display.display();
}
