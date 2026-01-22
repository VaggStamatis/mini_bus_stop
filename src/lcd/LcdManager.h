#pragma once
#include <Arduino.h>

enum LcdMode {
  LCD_BOOT,
  LCD_AP_MODE,
  LCD_WIFI_CONNECTING,
  LCD_WIFI_CONNECTED,
  LCD_WIFI_FAILED
};

class LcdManager {
public:
  static void begin();
  static void loop();

  static void setMode(LcdMode mode);
  static void setLine(uint8_t line, const String& text);

private:
  static void refresh();

  static LcdMode currentMode;
  static LcdMode lastMode;

  static String lines[2];
  static String lastLines[2];

  static unsigned long lastRefresh;
  static const unsigned long REFRESH_INTERVAL = 300;
};
