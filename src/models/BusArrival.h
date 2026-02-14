#pragma once
#include <Arduino.h>

struct BusArrival {
    String line;
    String destination;
    int minutes;
};