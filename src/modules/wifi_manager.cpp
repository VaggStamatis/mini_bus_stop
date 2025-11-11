// #include <WiFi.h>

// const char* ssid = "Gouelcam To Favela";
// const char* password = "kat0Gatisi@";

// void connect_to_local_wifi(){
//     Serial.begin(9600);

//     WiFi.mode(WIFI_STA);  // Set to Station mode
//     WiFi.begin(ssid, password);
//     Serial.println("Connecting ...");
//     while(WiFi.status() != WL_CONNECTED){
//         delay(500);
//         Serial.println(".");
//         Serial.println(WiFi.status());
//     }

//     Serial.println("");
//     Serial.println("Connected to WiFi network with IP Address: ");
//     Serial.println(WiFi.localIP());
// }

#include "wifi_manager.h"
#include <SPIFFS.h>
#include <api_request.h>

void WiFiManager::begin() {
    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS mount failed");
        return;
    }

    preferences.begin("wifi", false);
    String ssid = preferences.getString("ssid", "");
    String pass = preferences.getString("pass", "");
    preferences.end();

    if (ssid != "") {
        Serial.printf("Trying to connect to saved Wi-Fi: %s\n", ssid.c_str());
        WiFi.begin(ssid.c_str(), pass.c_str());
        if (WiFi.waitForConnectResult() == WL_CONNECTED) {
            Serial.println("Connected to Wi-Fi!");
            Serial.println(WiFi.localIP());
            return;
        }
    }

    startAPMode();
}

void WiFiManager::loop() {
    // dnsServer.processNextRequest();
    // server.handleClient();

    Serial.println("Fetching data for Bus Stop ΑΓΙΟΣ ΛΟΥΚΑΣ");
    get_request();
    delay(9000);
}

void WiFiManager::startAPMode() {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ap_ssid, ap_password);
    Serial.printf("AP Mode Started: %s\n", WiFi.softAPIP().toString().c_str());
    startCaptivePortal();
}

void WiFiManager::startCaptivePortal() {
    dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());
    server.on("/", std::bind(&WiFiManager::handleRoot, this));
    server.on("/save", HTTP_POST, std::bind(&WiFiManager::handleSave, this));
    server.serveStatic("/", SPIFFS, "/");
    server.begin();
    Serial.println("Captive portal running...");
}

void WiFiManager::handleRoot() {
    File file = SPIFFS.open("/index.html", "r");
    if (!file) {
        server.send(500, "text/plain", "File not found");
        return;
    }
    server.streamFile(file, "text/html");
    file.close();
}

void WiFiManager::handleSave() {
    String ssid = server.arg("ssid");
    String pass = server.arg("pass");
    String f1 = server.arg("f1");
    String f2 = server.arg("f2");
    String f3 = server.arg("f3");
    String f4 = server.arg("f4");

    preferences.begin("wifi", false);
    preferences.putString("ssid", ssid);
    preferences.putString("pass", pass);
    preferences.putString("f1", f1);
    preferences.putString("f2", f2);
    preferences.putString("f3", f3);
    preferences.putString("f4", f4);
    preferences.end();

    server.send(200, "text/html", "<html><body><h3>Saved! Rebooting...</h3></body></html>");
    delay(2000);
    ESP.restart();
}
