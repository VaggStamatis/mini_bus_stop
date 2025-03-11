#include <WiFi.h>

const char* ssid = "Gouelcam To Favela";
const char* password = "kat0Gatisi@";

void connect_to_local_wifi(){
    Serial.begin(9600);

    WiFi.mode(WIFI_STA);  // Set to Station mode
    WiFi.begin(ssid, password);
    Serial.println("Connecting ...");
    while(WiFi.status() != WL_CONNECTED){
        delay(500);
        Serial.println(".");
        Serial.println(WiFi.status());
    }

    Serial.println("");
    Serial.println("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
}