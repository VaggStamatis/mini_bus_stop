#include <HTTPClient.h>
#include <WiFi.h>
#include "api_request.h"
#include <ArduinoJson.h>

const char* busStop_arrivals = "http://telematics.oasa.gr/api/?act=getStopArrivals&p1=061061";
const char* busStop_information = "http://telematics.oasa.gr/api/?act=webRoutesForStop&p1=061061";


/*
  Combines the 2 input JsonDocuments by finding a matching column in the second 
  JsonDocumnet with the first one and printing the rest of the data based on the 
  matching column (in this case the matching column is route_code and RouteCode)

  *params 
    arrivals: stores the arrival times of the buses
    routes: stores the information of each route that specified stop serves
*/
void combineAndPrintRoutes(JsonDocument& arrivals, JsonDocument& routes) {
  for (JsonObject arrival : arrivals.as<JsonArray>()) {
    const char* route_code = arrival["route_code"];
    const char* vehicle = arrival["veh_code"];
    const char* time = arrival["btime2"];

    // Search for a matching route in doc_ids
    for (JsonObject route : routes.as<JsonArray>()) {
      const char* route_info_code = route["RouteCode"];

      if (strcmp(route_code, route_info_code) == 0) {
        const char* route_id = route["LineID"];
        const char* description = route["RouteDescrEng"];

        Serial.print("Id: ");
        Serial.print(route_id);
        Serial.print(" | Time: ");
        Serial.print(time);
        Serial.print(" | Description: ");
        Serial.println(description);
        break; // found the match, no need to search further
      }
    }
  }
}


void get_request(){
    Serial.begin(9600);
    if (WiFi.status() == WL_CONNECTED){

      // obtain the bus stop arrival times
        HTTPClient http;
        http.begin(busStop_arrivals);
        int httpResponseCode = http.GET();
        StaticJsonDocument<300> doc_time, doc_ids, result;
        
        if (httpResponseCode > 0) {
          String payload = http.getString();
          // Deserialize (parse) the JSON string into the object
          /*
            Response comes with 3 columns 
            route_code | veh_code | btime2
          */
          deserializeJson(doc_time, payload);
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
            String payload = httpInfo.getString();
            // Deserialize (parse) the JSON string into the object
            /*
              Response comes with 3 columns 
              RouteCode | RouteDescrEng | LineID
            */
            deserializeJson(doc_ids, payload);
          } else {
            Serial.print("Error code: ");
            Serial.println(httpResponseCodeInfo);
          }
          httpInfo.end();
      
      // combine two deserialized json objets to create final version of printable message
      combineAndPrintRoutes(doc_time, doc_ids);
    }else {
        Serial.println("Wi-Fi not connected");
    }
}
