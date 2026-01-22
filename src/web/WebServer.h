#pragma once
#include <WebServer.h>
#include "config/ConfigManager.h"

class WebUI {
public:
  static void begin();
  static void loop();

private:
  static WebServer server;
  static void handleRoot();
  static void handleSave();
};
