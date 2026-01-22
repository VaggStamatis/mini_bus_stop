#include "LcdManager.h"
#include <LiquidCrystal_I2C.h>

static LiquidCrystal_I2C lcd(0x27, 16, 2);

LcdMode LcdManager::currentMode = LCD_BOOT;
LcdMode LcdManager::lastMode = LCD_BOOT;

String LcdManager::lines[2] = {"", ""};
String LcdManager::lastLines[2] = {"", ""};

unsigned long LcdManager::lastRefresh = 0;

void LcdManager::begin() {
  lcd.init();
  lcd.backlight();

  setMode(LCD_BOOT);
  setLine(0, "ESP32 Booting");
  setLine(1, "Please wait...");
}

void LcdManager::setMode(LcdMode mode) {
  currentMode = mode;
}

void LcdManager::setLine(uint8_t line, const String& text) {
  if (line > 1) return;
  lines[line] = text;
}

void LcdManager::loop() {
  if (millis() - lastRefresh < REFRESH_INTERVAL)
    return;

  refresh();
  lastRefresh = millis();
}

void LcdManager::refresh() {
  bool changed = false;

  if (currentMode != lastMode) {
    lcd.clear();
    lastMode = currentMode;
    changed = true;
  }

  for (int i = 0; i < 2; i++) {
    if (lines[i] != lastLines[i]) {
      lcd.setCursor(0, i);
      lcd.print("                "); // clear line
      lcd.setCursor(0, i);
      lcd.print(lines[i].substring(0, 16));
      lastLines[i] = lines[i];
      changed = true;
    }
  }

  (void)changed;
}
