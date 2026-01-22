#include "WebServer.h"
#include <WiFi.h>
#include <SPIFFS.h>
#include "lcd/LcdManager.h"

WebServer WebUI::server(80);

void WebUI::begin() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP("ESP32-Setup");

  IPAddress ip = WiFi.softAPIP();
  Serial.print("AP started. IP: ");
  LcdManager::setLine(1, ip.toString());
  Serial.println(ip);

  server.on("/", HTTP_GET, handleRoot);
  server.on("/save", HTTP_POST, handleSave);

  server.begin();
}

void WebUI::loop() {
  server.handleClient();
}

void WebUI::handleRoot() {
  File f = SPIFFS.open("/index.html", "r");
  if (!f) {
    server.send(500, "text/plain", "index.html missing");
    return;
  }

  server.streamFile(f, "text/html");
  f.close();
}

void WebUI::handleSave() {
  AppConfig cfg;
  cfg.ssid = server.arg("ssid");
  cfg.password = server.arg("pass");
  cfg.busStops[0] = server.arg("BusStop1");
  cfg.busStops[1] = server.arg("BusStop2");
  cfg.busStops[2] = server.arg("BusStop3");
  cfg.busStops[3] = server.arg("BusStop4");

  ConfigManager::save(cfg);

  server.send(200, "text/html",
    "<h2>Saved! ESP32 will restart.</h2>");

  delay(1000);
  ESP.restart();
}
