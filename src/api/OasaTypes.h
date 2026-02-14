#pragma once
#include <Arduino.h>

struct ArrivalInfo {
  String line;
  int minutes;
};

struct RouteInfo {
  String line;
  String description;
};
