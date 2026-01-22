#include <Arduino.h>
#include "config/ConfigManager.h"
#include "web/WebServer.h"
#include "wifi/WiFiController.h"
#include "lcd/LcdManager.h"
#include "state/AppStateMachine.h"

AppConfig config;
bool apMode = false;

void setup() {
  Serial.begin(9600);

  ConfigManager::begin();
  LcdManager::begin();

  if (ConfigManager::load(config)) {
    LcdManager::setMode(LCD_WIFI_CONNECTING);
    LcdManager::setLine(0, "Connecting WiFi");
    LcdManager::setLine(1, config.ssid);

    WiFiController::begin();
    AppStateMachine::begin();
  } else {
    LcdManager::setMode(LCD_AP_MODE);
    LcdManager::setLine(0, "AP Mode");
    LcdManager::setLine(1, "192.168.4.1");

    WebUI::begin();
    apMode = true;
  }
}

void loop() {
  LcdManager::loop();

  if (apMode) {
    WebUI::loop();
    return;
  }

  WiFiController::loop();
  AppStateMachine::loop();
}
