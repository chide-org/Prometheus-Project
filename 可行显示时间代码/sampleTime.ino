#include <DS1302.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x20,16,2); // 设置LCD I2C地址
DS1302 rtc(4, 3, 2); // 设置DS1302的RST、SDA和SCL引脚

void setup() {
  Serial.begin(9600);
  rtc.halt(false); // 开启DS1302时钟
  lcd.init();
  lcd.backlight();
  Time priTime(2024,3,23,16,10,01,1);
  rtc.time(priTime);
}

void loop() {
  Time t = rtc.time(); // 获取当前时间
  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  lcd.print(t.hr);
  lcd.print(":");
  lcd.print(t.min);
  lcd.print(":");
  lcd.print(t.sec);
  lcd.setCursor(0, 1);
  lcd.print("Date: ");
  lcd.print(t.yr);
  lcd.print("/");
  lcd.print(t.mon);
  lcd.print("/");
  lcd.print(t.date);
  delay(1000); // 延迟1秒
}
