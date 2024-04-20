#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
LiquidCrystal_I2C lcd(0x20,16,2);
#define DHTPIN 8

#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup()
{
    Serial.begin(9600);
    dht.begin();
    lcd.init();
    lcd.backlight();
}

void loop()
{
    delay(2000);
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    Serial.print("Tempereture:");
    Serial.println((int)t);
    Serial.print("Humidity:");
    Serial.println((int)h);

    lcd.setCursor(0, 0);
    lcd.print("Tem:");
    lcd.print(t);
    lcd.setCursor(0, 1);
    lcd.print("Hum:");
    lcd.print(h);
}
