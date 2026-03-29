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
        String lineCode = obj["LineID"].as<String>();
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

        String line = toGreeklish(routesMap[routeCode].first);
        String destination = toGreeklish(routesMap[routeCode].second);

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

String ArrivalMerger::toGreeklish(const String& greek) {

    String result = greek;

    result.replace("Α", "A");
    result.replace("Β", "V");
    result.replace("Γ", "G");
    result.replace("Δ", "D");
    result.replace("Ε", "E");
    result.replace("Ζ", "Z");
    result.replace("Η", "I");
    result.replace("Θ", "TH");
    result.replace("Ι", "I");
    result.replace("Κ", "K");
    result.replace("Λ", "L");
    result.replace("Μ", "M");
    result.replace("Ν", "N");
    result.replace("Ξ", "X");
    result.replace("Ο", "O");
    result.replace("Π", "P");
    result.replace("Ρ", "R");
    result.replace("Σ", "S");
    result.replace("Τ", "T");
    result.replace("Υ", "Y");
    result.replace("Φ", "F");
    result.replace("Χ", "CH");
    result.replace("Ψ", "PS");
    result.replace("Ω", "O");

    // lowercase (optional but useful)
    result.replace("α", "a");
    result.replace("β", "v");
    result.replace("γ", "g");
    result.replace("δ", "d");
    result.replace("ε", "e");
    result.replace("ζ", "z");
    result.replace("η", "i");
    result.replace("θ", "th");
    result.replace("ι", "i");
    result.replace("κ", "k");
    result.replace("λ", "l");
    result.replace("μ", "m");
    result.replace("ν", "n");
    result.replace("ξ", "x");
    result.replace("ο", "o");
    result.replace("π", "p");
    result.replace("ρ", "r");
    result.replace("σ", "s");
    result.replace("ς", "s");
    result.replace("τ", "t");
    result.replace("υ", "y");
    result.replace("φ", "f");
    result.replace("χ", "ch");
    result.replace("ψ", "ps");
    result.replace("ω", "o");

    return result;
}