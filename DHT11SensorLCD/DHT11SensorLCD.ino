#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
void setup() {
  lcd.begin();
  dht.begin();
}

void loop() {
  lcd.clear();     
  // read humidity
  float humi  = dht.readHumidity();
  // read temperature as Celsius
  float tempC = dht.readTemperature();

    // check if any reads failed
  if (isnan(humi) || isnan(tempC)) {
    lcd.setCursor(5, 0);
    lcd.print("Failed");
  } else {
    lcd.setCursor(2, 0);
    lcd.print("Humi : ");
    lcd.print(humi);
    lcd.print("%");
    lcd.setCursor(1, 1);
    lcd.print("Temp : ");
    lcd.print(tempC);
    lcd.print(" C");
  }
  delay(1000);                         
}
