#include <Wire.h>
#include <LiquidCrystal_I2C.h>//显示屏
#include <DHT.h>//温湿度传感器
#include <Adafruit_Sensor.h>
#include <DS1302.h>//时钟

LiquidCrystal_I2C lcd(0x20,16,2);//显示屏

#define DHTPIN 8
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);//温湿度传感器

DS1302 rtc(4, 3, 2); // 设置DS1302的RST、SDA和SCL引脚

int s = 0;//自动切换时间
int temp = 5000;//切换间隔时长

void setup()
{
    Serial.begin(9600);
    
    rtc.halt(false); 
    Time priTime(2024,3,23,16,10,01,1);
    rtc.time(priTime);// 开启DS1302时钟
    
    dht.begin();//开启温湿度传感器
    
    lcd.init();
    lcd.backlight();//显示屏初始化

    
}

void loop()
{
    
    if(s>=0&&s<temp){
      if(s==0)lcd.clear();
      
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

      delay(1000);
      s=s+1000;
    }
    else if(s>=temp&&s<temp*2){
      if(s==temp)lcd.clear();
      
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
      delay(1000);
      s=s+1000;
    }
    else {s = 0;}
}
