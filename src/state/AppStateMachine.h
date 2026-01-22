#pragma once
#include <Arduino.h>

enum AppState {
  APP_INIT,
  APP_WAIT_WIFI,
  APP_READY,
  APP_NEXT_BUS_STOP,
  APP_FETCHING_DATA,
  APP_DISPLAY_DATA,
  APP_ERROR
};

class AppStateMachine {
public:
  static void begin();
  static void loop();

private:
  static AppState state;
  static unsigned long stateTimestamp;

  static uint8_t currentBusStopIndex;

  static void changeState(AppState newState);
};
