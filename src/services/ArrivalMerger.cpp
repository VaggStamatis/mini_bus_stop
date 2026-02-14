#include "ArrivalMerger.h"
#include <ArduinoJson.h>

std::vector<BusArrival> ArrivalMerger::merge(
    const String &arrivalsJson,
    const String &routesJson)
{
    std::vector<BusArrival> result;

    /* ---------------- PHASE 1: ARRIVALS ---------------- */
    std::map<String, std::vector<int>> arrivalsMap;

    DynamicJsonDocument arrivalsDoc(8192);
    deserializeJson(arrivalsDoc, arrivalsJson);

    for (JsonObject obj : arrivalsDoc.as<JsonArray>())
    {
        String routeCode = obj["route_code"].as<String>();
        int minutes = obj["btime2"].as<int>();

        arrivalsMap[routeCode].push_back(minutes);
    }

    Serial.println("Parsed arrivals OK");

    /* ---------------- PHASE 2: ROUTES ---------------- */
    std::map<String, std::pair<String, String>> routesMap;
    // route_code -> (line, destination)

    DynamicJsonDocument routesDoc(20000);
    deserializeJson(routesDoc, routesJson);

    for (JsonObject obj : routesDoc.as<JsonArray>())
    {

        String routeCode = obj["RouteCode"].as<String>();
        String lineCode = obj["LineCode"].as<String>();
        String lineDescr = obj["LineDescr"].as<String>();

        String destination = extractDestination(lineDescr);

        routesMap[routeCode] = {lineCode, destination};
    }

    Serial.println("Parsed routes OK");

    /* ---------------- PHASE 3: MERGE ---------------- */

    for (auto const &arrivalPair : arrivalsMap)
    {

        String routeCode = arrivalPair.first;

        if (routesMap.find(routeCode) == routesMap.end())
            continue;

        String line = routesMap[routeCode].first;
        String destination = routesMap[routeCode].second;

        for (int minutes : arrivalPair.second)
        {
            BusArrival bus;
            bus.line = line;
            bus.destination = destination;
            bus.minutes = minutes;

            result.push_back(bus);
        }
    }

    Serial.println("Merge complete");
    return result;
}

/* Extract last stop (Option A) */
String ArrivalMerger::extractDestination(const String &lineDescr)
{

    int idx = lineDescr.lastIndexOf('-');
    if (idx == -1)
        return lineDescr;

    String dest = lineDescr.substring(idx + 1);
    dest.trim();
    return dest;
}