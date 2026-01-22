#include "WiFiController.h"
#include <WiFi.h>
#include "config/ConfigManager.h"

WiFiState WiFiController::state = WIFI_IDLE;
unsigned long WiFiController::lastAttempt = 0;
uint8_t WiFiController::retryCount = 0;

void WiFiController::begin() {
  AppConfig cfg;

  if (!ConfigManager::load(cfg)) {
    state = WIFI_FAILED;
    return;
  }

  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true);
  delay(100);

  Serial.print("Connecting to SSID: ");
  Serial.println(cfg.ssid);

  WiFi.begin(cfg.ssid.c_str(), cfg.password.c_str());

  state = WIFI_CONNECTING;
  lastAttempt = millis();
  retryCount = 0;
}

void WiFiController::loop() {
  if (state == WIFI_CONNECTED || state == WIFI_FAILED)
    return;

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("WiFi connected. IP: ");
    Serial.println(WiFi.localIP());
    state = WIFI_CONNECTED;
    return;
  }

  if (millis() - lastAttempt > RETRY_INTERVAL) {
    lastAttempt = millis();
    retryCount++;

    Serial.println("WiFi retry...");

    if (retryCount >= MAX_RETRIES) {
      Serial.println("WiFi failed.");
      WiFi.disconnect();
      state = WIFI_FAILED;
      return;
    }

    WiFi.reconnect();
  }
}

WiFiState WiFiController::getState() {
  return state;
}

bool WiFiController::isConnected() {
  return state == WIFI_CONNECTED;
}

String WiFiController::ip() {
  return isConnected() ? WiFi.localIP().toString() : "";
}
