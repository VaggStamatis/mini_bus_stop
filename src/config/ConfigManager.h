#pragma once
#include <Arduino.h>

struct AppConfig {
  String ssid;
  String password;
  String busStops[4];
};

class ConfigManager {
public:
  static bool begin();
  static bool save(const AppConfig& cfg);
  static bool load(AppConfig& cfg);
};
