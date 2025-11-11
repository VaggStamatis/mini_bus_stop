#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <wifi_manager.h>
#include <api_request.h>
#include <lcd_i2c.h>

WiFiManager wifiManager;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd_init();
  // connect_to_local_wifi();
  wifiManager.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  wifiManager.loop();
  // Serial.println("Fetching data for Bus Stop ΑΓΙΟΣ ΛΟΥΚΑΣ");
  // get_request();
}
