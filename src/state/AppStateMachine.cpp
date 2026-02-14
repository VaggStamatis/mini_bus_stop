#include "state/AppStateMachine.h"
#include "wifi/WiFiController.h"
#include "lcd/LcdManager.h"
#include "config/ConfigManager.h"
#include "api/OasaClient.h"

AppState AppStateMachine::state = APP_INIT;
unsigned long AppStateMachine::stateTimestamp = 0;
uint8_t AppStateMachine::currentBusStopIndex = 0;

static AppConfig config;
OasaClient AppStateMachine::apiClient;
void AppStateMachine::begin() {
  ConfigManager::load(config);
  changeState(APP_WAIT_WIFI);
}

void AppStateMachine::loop() {

  switch (state) {

    case APP_WAIT_WIFI: {
      if (WiFiController::getState() == WIFI_CONNECTED) {
        changeState(APP_READY);
      } 
      else if (WiFiController::getState() == WIFI_FAILED) {
        changeState(APP_ERROR);
      }
      break;
    }

    case APP_READY: {
      changeState(APP_NEXT_BUS_STOP);
      break;
    }

    case APP_NEXT_BUS_STOP: {
      if (config.busStops[currentBusStopIndex].length() == 0) {
        currentBusStopIndex = (currentBusStopIndex + 1) % 4;
        break;
      }

      LcdManager::setLine(0, "Bus Stop");
      LcdManager::setLine(1, config.busStops[currentBusStopIndex]);

      changeState(APP_FETCHING_DATA);
      break;
    }

    case APP_FETCHING_DATA: {
      String stop = config.busStops[currentBusStopIndex];

      Serial.println("================================");
      Serial.print("Fetching data for stop: ");
      Serial.println(stop);

      apiClient.fetchAndMerge(stop);

      changeState(APP_DISPLAY_DATA);
      break;
    }

    case APP_DISPLAY_DATA: {
      if (millis() - stateTimestamp > 5000) {
        currentBusStopIndex = (currentBusStopIndex + 1) % 4;
        changeState(APP_NEXT_BUS_STOP);
      }
      break;
    }

    case APP_ERROR: {
      LcdManager::setLine(0, "App Error");
      LcdManager::setLine(1, "WiFi failed");
      break;
    }

    default:
      break;
  }
}

void AppStateMachine::changeState(AppState newState) {
  state = newState;
  stateTimestamp = millis();
}