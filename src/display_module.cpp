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

// Track which display page we're showing
enum DisplayPage
{
  CLOCK_PAGE,
  SENSORS_PAGE,
  SYSTEM_PAGE
};

DisplayPage currentPage = CLOCK_PAGE;
unsigned long lastPageChange = 0;
const unsigned long pageRotationInterval = 5000; // Rotate pages every 5 seconds

// Battery icon dimensions
#define BATT_WIDTH 10
#define BATT_HEIGHT 5

// Helper: Convert 24-hour format to 12-hour format.
int timeto12(int hourin24)
{
  if (hourin24 == 0)
    return 12;
  if (hourin24 > 12)
    return hourin24 - 12;
  return hourin24;
}

void initDisplay()
{
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    while (1)
      ;
  }
  display.clearDisplay();

  // Show splash screen
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(15, 0);
  display.println(F("CUMIN LANDER"));
  display.setCursor(20, 12);
  display.println(F("STARTING..."));

  // Draw a border
  display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);

  display.display();
  delay(2000);
}

// Draw a battery icon with specified level (0-100%)
void drawBattery(int x, int y, int level)
{
  // Constrain level between 0-100
  level = constrain(level, 0, 100);

  // Draw battery outline
  display.drawRect(x, y, BATT_WIDTH, BATT_HEIGHT, SSD1306_WHITE);
  display.drawPixel(x + BATT_WIDTH, y + 1, SSD1306_WHITE);
  display.drawPixel(x + BATT_WIDTH, y + BATT_HEIGHT - 2, SSD1306_WHITE);

  // Draw battery level
  int fillWidth = map(level, 0, 100, 0, BATT_WIDTH - 2);
  if (fillWidth > 0)
  {
    display.fillRect(x + 1, y + 1, fillWidth, BATT_HEIGHT - 2, SSD1306_WHITE);
  }
}

// Draw the clock page
void drawClockPage()
{
  canvas.fillScreen(0);
  canvas.setTextColor(SSD1306_WHITE, SSD1306_BLACK);

  time_t t = now();

  // Large clock display in the center
  canvas.setFont(&DSEG7_Classic_Mini_Regular_15);

  // Format hours, minutes and seconds
  char timeStr[9];
  sprintf(timeStr, "%02d:%02d:%02d", timeto12(hour(t)), minute(t), second(t));

  // Calculate center position and draw time
  int16_t x1, y1;
  uint16_t w, h;
  canvas.getTextBounds(timeStr, 0, 0, &x1, &y1, &w, &h);
  int centerX = (SCREEN_WIDTH - w) / 2;
  canvas.setCursor(centerX, 20);
  canvas.print(timeStr);

  // Draw AM/PM indicator
  canvas.setFont();
  canvas.setTextSize(1);
  canvas.setCursor(centerX + w + 3, 12);
  canvas.print(isPM() ? F("PM") : F("AM"));

  // Draw the date on top
  char dateStr[11];
  sprintf(dateStr, "%02d/%02d/%04d", day(t), month(t), year(t));
  canvas.getTextBounds(dateStr, 0, 0, &x1, &y1, &w, &h);
  centerX = (SCREEN_WIDTH - w) / 2;
  canvas.setCursor(centerX, 6);
  canvas.print(dateStr);

  // Draw the battery level at top right
  drawBattery(SCREEN_WIDTH - BATT_WIDTH - 3, 1, 85); // Assuming 85% battery

  display.drawBitmap(0, 0, canvas.getBuffer(), 128, 32, 1, 0);
}

// Draw the sensors page
void drawSensorsPage()
{
  display.clearDisplay();

  // Read sensor values
  float temperature = bme.readTemperature();
  int humidity = bme.readHumidity();
  int pressure = bme.readPressure() / 100.0F;

  // Title
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(32, 0);
  display.println(F("SENSOR DATA"));
  display.drawLine(0, 9, SCREEN_WIDTH, 9, SSD1306_WHITE);

  // Temperature with icon
  display.setCursor(5, 12);
  display.print(F("T:"));
  display.setCursor(15, 12);
  display.print(temperature, 1);
  display.print(F(" C"));

  // Draw thermometer icon
  display.drawCircle(3, 12, 2, SSD1306_WHITE);
  display.drawLine(3, 14, 3, 18, SSD1306_WHITE);

  // Humidity with icon
  display.setCursor(5, 22);
  display.print(F("H:"));
  display.setCursor(15, 22);
  display.print(humidity);
  display.print(F("%"));

  // Draw drop icon
  display.drawPixel(3, 21, SSD1306_WHITE);
  display.drawLine(2, 22, 4, 22, SSD1306_WHITE);
  display.drawLine(1, 23, 5, 23, SSD1306_WHITE);
  display.drawLine(2, 24, 4, 24, SSD1306_WHITE);
  display.drawPixel(3, 25, SSD1306_WHITE);

  // Pressure
  display.setCursor(70, 12);
  display.print(F("P:"));
  display.setCursor(82, 12);
  display.print(pressure);
  display.setCursor(82, 22);
  display.print(F("hPa"));

  // Draw pressure icon
  display.drawRect(72, 22, 4, 5, SSD1306_WHITE);
  display.drawLine(74, 22, 74, 20, SSD1306_WHITE);
  display.drawLine(74, 20, 76, 18, SSD1306_WHITE);

  display.display();
}

// Draw the system page
void drawSystemPage()
{
  display.clearDisplay();

  // Title
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(32, 0);
  display.println(F("SYSTEM INFO"));
  display.drawLine(0, 9, SCREEN_WIDTH, 9, SSD1306_WHITE);

  // Draw BLE status
  display.setCursor(5, 12);
  display.print(F("BLE: "));
  display.print(F("Active"));

  // Draw battery level
  display.setCursor(5, 22);
  display.print(F("BAT: 85%"));
  drawBattery(70, 22, 85);

  // Draw uptime
  unsigned long uptime = millis() / 1000;
  int hours = uptime / 3600;
  int mins = (uptime % 3600) / 60;
  int secs = uptime % 60;

  display.setCursor(70, 12);
  display.print(F("UP: "));
  if (hours < 10)
    display.print(F("0"));
  display.print(hours);
  display.print(F(":"));
  if (mins < 10)
    display.print(F("0"));
  display.print(mins);

  display.display();
}

void updateDisplay()
{
  // Check if it's time to change pages
  if (millis() - lastPageChange > pageRotationInterval)
  {
    lastPageChange = millis();
    currentPage = (DisplayPage)((currentPage + 1) % 3);
  }

  // Display the current page
  switch (currentPage)
  {
  case CLOCK_PAGE:
    drawClockPage();
    break;
  case SENSORS_PAGE:
    drawSensorsPage();
    break;
  case SYSTEM_PAGE:
    drawSystemPage();
    break;
  }
}