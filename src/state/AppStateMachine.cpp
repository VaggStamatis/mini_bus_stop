#include "state/AppStateMachine.h"
#include "wifi/WiFiController.h"
#include "lcd/LcdManager.h"
#include "config/ConfigManager.h"
#include "api/ArrivalStore.h"
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

      static bool shown = false;

      // Skip empty stops
      if (config.busStops[currentBusStopIndex].length() == 0) {
        currentBusStopIndex = (currentBusStopIndex + 1) % 4;
        break;
      }

      // 🖨️ Show ONLY once when entering state
      if (!shown) {
        LcdManager::setLine(0, "Bus Stop");
        LcdManager::setLine(1, config.busStops[currentBusStopIndex]);
        shown = true;
      }

      // ⏱️ Wait 2.5 seconds before fetching
      if (millis() - stateTimestamp > 2500) {
        shown = false;  // reset for next time
        changeState(APP_FETCHING_DATA);
      }

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
      static unsigned long lastRotate = 0;
      static int displayIndex = 0;

      auto &arrivals = ArrivalStore::arrivals;

      if (arrivals.empty()) {
        LcdManager::setLine(0, "No arrivals");
        LcdManager::setLine(1, "");
      } 
      else {

        // 🔁 Rotate every 3 seconds
        if (millis() - lastRotate > 3000) {
          displayIndex++;

          if (displayIndex >= arrivals.size()) {
            displayIndex = 0;
          }

          lastRotate = millis();
        }

        // 🚌 Current bus
        auto &bus = arrivals[displayIndex];

        // Line 1 → "4 in 3 min"
        String line1 = bus.line + " in " + String(bus.minutes) + " min";

        // Line 2 → destination
        String line2 = bus.destination;

        // ✂️ Ensure 16 char max
        line1 = line1.substring(0, 16);
        line2 = line2.substring(0, 16);

        LcdManager::setLine(0, line1);
        LcdManager::setLine(1, line2);
      }

      // ⏱️ Stay longer on each stop now (important!)
      if (millis() - stateTimestamp > 15000) {  // 👈 increased from 5s
        displayIndex = 0;
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