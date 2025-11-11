// // wifi_module.h
// #ifndef WIFI_MODULE_H
// #define WIFI_MODULE_H
// DEPRECATED For now
// void connect_to_local_wifi();  // Declare the method here

// #endif


#pragma once
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <Preferences.h>

class WiFiManager {
public:
    void begin();
    void loop();

private:
    void startAPMode();
    void startCaptivePortal();
    void handleRoot();
    void handleSave();
    void connectToSavedWiFi();

    const byte DNS_PORT = 53;
    const char* ap_ssid = "ESP32_Config";
    const char* ap_password = "mini_stop123";

    DNSServer dnsServer;
    WebServer server{80};
    Preferences preferences;
};
