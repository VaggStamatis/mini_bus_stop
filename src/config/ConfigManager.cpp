#include "ConfigManager.h"
#include <SPIFFS.h>
#include <ArduinoJson.h>

static const char* CONFIG_FILE = "/config.json";

bool ConfigManager::begin() {
  return SPIFFS.begin(true);
}

bool ConfigManager::save(const AppConfig& cfg) {
  File f = SPIFFS.open(CONFIG_FILE, "w");
  if (!f) return false;

  StaticJsonDocument<256> doc;
  doc["ssid"] = cfg.ssid;
  doc["password"] = cfg.password;

  JsonArray arr = doc.createNestedArray("busStops");
  for (int i = 0; i < 4; i++) {
    arr.add(cfg.busStops[i]);
  }

  serializeJson(doc, f);
  f.close();
  return true;
}

bool ConfigManager::load(AppConfig& cfg) {
  if (!SPIFFS.exists(CONFIG_FILE)) return false;

  File f = SPIFFS.open(CONFIG_FILE, "r");
  if (!f) return false;

  StaticJsonDocument<256> doc;
  DeserializationError err = deserializeJson(doc, f);
  f.close();

  if (err) return false;

  cfg.ssid = doc["ssid"].as<String>();
  cfg.password = doc["password"].as<String>();

  for (int i = 0; i < 4; i++) {
    cfg.busStops[i] = doc["busStops"][i].as<String>();
  }

  return true;
}
