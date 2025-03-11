#include <HTTPClient.h>
#include <WiFi.h>
#include "api_request.h"
#include <ArduinoJson.h>
const char* busStop_arrivals = "http://telematics.oasa.gr/api/?act=getStopArrivals&p1=061061";
const char* busStop_information = "http://telematics.oasa.gr/api/?act=webRoutesForStop&p1=061061";

void get_request(){
    Serial.begin(9600);
    if (WiFi.status() == WL_CONNECTED){

      // obtain the bus stop arrival times
        HTTPClient http;
        http.begin(busStop_arrivals);
        int httpResponseCode = http.GET();
        
        
        if (httpResponseCode > 0) {
          Serial.print("HTTP Response code: ");
          Serial.println(httpResponseCode);
          String payload = http.getString();

          JsonDocument doc;
          // Deserialize (parse) the JSON string into the object
          DeserializationError error = deserializeJson(doc, payload);
          // Print data #beautify
          
          for (JsonObject obj : doc.as<JsonArray>()) {
            Serial.print("Route: ");
            Serial.print(obj["route_code"].as<const char*>());
            Serial.print(" | Vehicle: ");
            Serial.print(obj["veh_code"].as<const char*>());
            Serial.print(" | Time: ");
            Serial.println(obj["btime2"].as<const char*>());
          }
          
        } else {
          Serial.print("Error code: ");
          Serial.println(httpResponseCode);
        }
        http.end();
        
        // Obtain the bus stop information
        HTTPClient httpInfo;
        httpInfo.begin(busStop_information);
        int httpResponseCodeInfo = httpInfo.GET();

        if (httpResponseCodeInfo > 0) {
            Serial.print("HTTP Response code: ");
            Serial.println(httpResponseCodeInfo);
            String payload = httpInfo.getString();
            // Serial.println("Response:");
            // Serial.println(payload);
            JsonDocument doc;
            // Deserialize (parse) the JSON string into the object
            DeserializationError error = deserializeJson(doc, payload);
            for (JsonObject obj : doc.as<JsonArray>()) {
              Serial.print("Route: ");
              Serial.print(obj["RouteCode"].as<const char*>());
              Serial.print(" | Description: ");
              Serial.print(obj["RouteDescrEng"].as<const char*>());
              Serial.print(" | Id: ");
              Serial.println(obj["LineID"].as<const char*>());
            }
          } else {
            Serial.print("Error code: ");
            Serial.println(httpResponseCodeInfo);
          }
          httpInfo.end();

    }else {
        Serial.println("Wi-Fi not connected");
    }
}