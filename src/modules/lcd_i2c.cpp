#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>
#include "lcd_i2c.h"

int rows;
int columns;
LiquidCrystal_I2C lcd(0x27, columns, rows);

void lcd_init(){
  Wire.begin();
  lcd.init();
  lcd.backlight();
  lcd.clear();
}
void print2LCD(JsonDocument& arrivals, JsonDocument& routes){
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

        lcd.clear();

        //Line 1: ID + Time
        lcd.setCursor(0,0);
        lcd.print ("ID:");
        lcd.print(route_id);
        lcd.print(" Est_T:");
        lcd.print(time);

        // Line 2: Line Description
        lcd.setCursor(0,1);
        lcd.print("Route");
        // lcd.print(description);

        delay(2000);
        break; // found the match, no need to search further
      }
    }
  }
}
