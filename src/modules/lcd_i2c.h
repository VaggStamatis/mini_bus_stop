// lcd_i2c.h
#ifndef LCI_I2C_H
#define LCI_I2C_H
#include  <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>
void print2LCD(JsonDocument& arrivals, JsonDocument& routes);
void lcd_init();
#endif