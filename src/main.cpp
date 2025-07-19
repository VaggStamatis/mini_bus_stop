#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "./modules/wifi_module.h"
#include "./modules/api_request.h"
#include "./modules/lcd_i2c.h"


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd_init();
  connect_to_local_wifi();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Fetching data for Bus Stop ΑΓΙΟΣ ΛΟΥΚΑΣ");
  get_request();
  delay(9000);
}
