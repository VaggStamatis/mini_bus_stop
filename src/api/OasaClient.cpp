#include "OasaClient.h"
#include "../services/ArrivalMerger.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>

bool OasaClient::fetchArrivals(const String &stopCode, String &response)
{
  char url[128];
  snprintf(url, sizeof(url),
           "http://telematics.oasa.gr/api/?act=getStopArrivals&p1=%s",
           stopCode.c_str());

  if (!httpGet(url, response))
  {
    Serial.println("[OASA] Arrivals HTTP failed");
    return false;
  }

  Serial.println("=== ARRIVALS RAW ===");
  // Serial.println(response);

  StaticJsonDocument<2048> doc;
  if (deserializeJson(doc, response))
  {
    Serial.println("[OASA] Arrivals JSON error");
    return false;
  }
  return true;
}

bool OasaClient::fetchRoutes(const String &stopCode, String &response)
{
  char url[128];
  snprintf(url, sizeof(url),
           "http://telematics.oasa.gr/api/?act=webRoutesForStop&p1=%s",
           stopCode.c_str());

  if (!httpGet(url, response))
  {
    Serial.println("[OASA] Routes HTTP failed");
    return false;
  }

  Serial.println("=== ROUTES RAW ===");
  // Serial.println(response);

  StaticJsonDocument<4096> doc;
  if (deserializeJson(doc, response))
  {
    Serial.println("[OASA] Routes JSON error");
    return false;
  }

  return true;
}

bool OasaClient::fetchAndMerge(const String &stopCode)
{
    String arrivalsJson;
    String routesJson;

    if (!fetchArrivals(stopCode, arrivalsJson))
        return false;

    if (!fetchRoutes(stopCode, routesJson))
        return false;

    auto merged = ArrivalMerger::merge(arrivalsJson, routesJson);

    Serial.println("===== MERGED RESULT =====");

    for (auto &bus : merged)
    {
        Serial.print(bus.line);
        Serial.print(" -> ");
        Serial.print(bus.destination);
        Serial.print(" in ");
        Serial.print(bus.minutes);
        Serial.println(" min");
    }

    // TODO fix the result print to include the bus code A2, 5, 11 not the bust_id 1072, 879
    /*
      ===== MERGED RESULT =====
      1072 -> ΤΖΙΤΖΙΦΙΕΣ in 12 min
      879 -> ΔΑΦΝΗ in 6 min
      1071 -> ΣΤ. ΑΓ. ΙΩΑΝΝΗΣ in 10 min
      1760 -> ΜΟΣΧΑΤΟ (ΕΩΣ ΛΕΩΦ.ΕΛ.ΒΕΝΙΖΕΛΟΥ) in 21 min
      1421 -> ΒΟΥΛΑ (ΜΕΣΩ Λ. ΑΜΦΙΘΕΑΣ) in 12 min
      1421 -> ΒΟΥΛΑ (ΜΕΣΩ Λ. ΑΜΦΙΘΕΑΣ) in 25 min
      1421 -> ΒΟΥΛΑ (ΜΕΣΩ Λ. ΑΜΦΙΘΕΑΣ) in 51 min
    */
    return true;
}

bool OasaClient::httpGet(const String &url, String &response)
{
  HTTPClient http;
  http.begin(url);

  http.setReuse(false);           // <-- IMPORTANT (no keep-alive)
  http.addHeader("Connection", "close");

  int code = http.GET();
  if (code <= 0)
  {
    http.end();
    return false;
  }

  response = http.getString();

  http.end();
  return true;
}
