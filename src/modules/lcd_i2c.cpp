#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>
#include "../include/lcd_i2c.h"

int rows = 2;
int columns = 16;
LiquidCrystal_I2C lcd(0x27, columns, rows);

void lcd_init(){
  Wire.begin();
  lcd.init();
  lcd.backlight();
  lcd.clear();
}

/*
  Standard LCD monitors can't handle Greek Alphabet characters
  Function to transform the route_id greek letters to latin characters
  recognised from lcd monitors
*/
String normalizeGreekToAscii(const char* input) {
  String text = String(input);

  // Greek capital letters with Latin equivalents
  text.replace("Α", "A"); // Alpha
  text.replace("Β", "B"); // Beta
  text.replace("Ε", "E"); // Epsilon
  text.replace("Ζ", "Z"); // Zeta
  text.replace("Η", "H"); // Eta
  text.replace("Ι", "I"); // Iota
  text.replace("Κ", "K"); // Kappa
  text.replace("Μ", "M"); // Mu
  text.replace("Ν", "N"); // Nu
  text.replace("Ο", "O"); // Omicron
  text.replace("Ρ", "P"); // Rho
  text.replace("Τ", "T"); // Tau
  text.replace("Υ", "Y"); // Upsilon
  text.replace("Χ", "X"); // Chi
  text.replace("Δ", "D"); // Delta (approx)
  text.replace("Λ", "L"); // Lambda (approx)
  text.replace("Θ", "Th"); // Theta (2 chars)
  text.replace("Ξ", "Ks"); // Xi (2 chars)
  text.replace("Σ", "S"); // Sigma
  text.replace("Φ", "F"); // Phi
  text.replace("Ψ", "Ps"); // Psi
  text.replace("Ω", "W"); // Omega (or "O" or "W")

  return text;
}


String transformDescription(String fullDescription) {
  int firstDash = fullDescription.indexOf(" - ");
  int lastDash = fullDescription.lastIndexOf(" - ");

  // If there's no dash, return as-is (or fallback)
  if (firstDash == -1 || lastDash == -1 || firstDash == lastDash) {
    return fullDescription.substring(0, 16);  // Safe fallback
  }

  // Extract start and end points
  String start = fullDescription.substring(0, firstDash);
  String end = fullDescription.substring(lastDash + 3); // Skip past " - "

  // Trim and limit to 7 characters
  start.trim(); end.trim();
  if (start.length() > 7) start = start.substring(0, 7);
  if (end.length() > 7) end = end.substring(0, 7);

  return start + "->" + end;
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
        String id_clean = normalizeGreekToAscii(route_id);
        // Line 1: ID + Time
        lcd.setCursor(0,0);
        lcd.print ("ID:");
        lcd.print(id_clean);
        lcd.print(" Est_T:");
        lcd.print(time);


        // Line 2: Line Description
        lcd.setCursor(0,1);
        String shortDesc = transformDescription(description);
        lcd.print(shortDesc);

        delay(2000);
        break; // found the match, no need to search further
      }
    }
  }
}


