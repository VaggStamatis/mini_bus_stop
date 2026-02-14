#pragma once
#include <Arduino.h>
#include "OasaTypes.h"

class OasaClient {
public:
  bool fetchArrivals(const String &stopCode, String &response);
  bool fetchRoutes(const String &stopCode, String &response);
  bool fetchAndMerge(const String &stopCode);

private:
  static bool httpGet(const String& url, String& response);
};
