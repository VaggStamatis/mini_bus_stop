#pragma once
#include <Arduino.h>

enum WiFiState {
  WIFI_IDLE,
  WIFI_CONNECTING,
  WIFI_CONNECTED,
  WIFI_FAILED
};

class WiFiController {
public:
  static void begin();
  static void loop();

  static WiFiState getState();
  static bool isConnected();
  static String ip();

private:
  static WiFiState state;
  static unsigned long lastAttempt;
  static uint8_t retryCount;

  static const unsigned long RETRY_INTERVAL = 5000;
  static const uint8_t MAX_RETRIES = 10;

  static void startConnection();
};
