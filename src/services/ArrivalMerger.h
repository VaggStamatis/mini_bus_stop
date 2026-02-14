#pragma once
#include <Arduino.h>
#include <vector>
#include <map>
#include "../models/BusArrival.h"

class ArrivalMerger {
public:
    static std::vector<BusArrival> merge(
        const String& arrivalsJson,
        const String& routesJson
    );

private:
    static String extractDestination(const String& lineDescr);
};