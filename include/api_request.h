// api_request.h
#ifndef API_REQUEST_H
#define API_REQUEST_H
#include <ArduinoJson.h>

void combineAndPrintRoutes(JsonDocument& arrivals, JsonDocument& routes);
void get_request();  // Declare the method here

#endif
