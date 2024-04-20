#include <DS1302.h> //时钟模块
#include <Wire.h>
#include <LiquidCrystal_I2C.h> //显示屏模块
#include <Servo.h>             //舵机
#include <DHT.h>               //温湿度传感器
#include <Adafruit_Sensor.h>

int pos_1 = 90;
int pos_2 = 90;
Servo servo_1;
Servo servo_2;
Servo servo_3;

LiquidCrystal_I2C lcd(0x20, 16, 2); // 设置LCD I2C地址
DS1302 rtc(4, 3, 2);                // 设置DS1302的RST、SDA和SCL引脚
#define DHTPIN 9
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE); // 温湿度传感器
int hz = 50;              // 刷新间隔时间ms
int temp = 3000;          // 切换间隔时长
int currentMin = NULL;    // 分钟记录，当分钟变化时切换显示内容
int currentHour = 10;     // 小时记录，当小时变化时执行清洗模块
int step = 1;             // 每次旋转修改的角度
int upmax = 130;          // 上舵机的边界角度
int upmin = 60;
int downmax = 170; // 下舵机的边界角度
int downmin = 10;
int stupid = 10;          // 公差
int limitLight = 150;     // 使太阳能板运行的最小光照强度
int cleanStartAngle = 70; // 清洁舵机起始角度
int cleanEndAngle = 150;  // 清洁舵机结束角度
int waterPIN = 0;         // 继电器水泵端口

void setup()
{
    servo_1.write(pos_1);
    servo_2.write(pos_2);
    servo_3.write(cleanStartAngle);
    Serial.begin(9600);
    pinMode(A0, INPUT); // 与A3相对
    pinMode(A1, INPUT); // 与A2相对
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);
    pinMode(A5, OUTPUT);
    servo_1.attach(11); // 上舵机
    servo_2.attach(12); // 下舵机
    servo_3.attach(13); // 清洁舵机
    servo_1.write(pos_1);
    servo_2.write(pos_2);

    rtc.halt(false); // 开启DS1302时钟
    lcd.init();
    lcd.backlight();
    Time priTime(2024, 3, 30, 10, 50, 0, 1);
    rtc.time(priTime);
    dht.begin(); // 开启温湿度传感器
    delay(1000);
}

void loop()
{
    delay(hz);
    LIGHT();
    Time nowtime = rtc.time(); // 获取当前时间
    if (nowtime.min != currentMin)
    {
        if (nowtime.min % 2 == 0)
            TIME(nowtime);
        else
            DHT();
        currentMin = nowtime.min;
    }
    if (nowtime.hr == currentHour)
    {
        CLEAN();
        currentHour = nowtime.hr + 1;
    }
}

void LIGHT()
{

    int b1 = analogRead(A0); // 下舵机
    int a1 = analogRead(A1); // 上舵机
    int a2 = analogRead(A2);
    int b2 = analogRead(A3);
    int a = (a1 - a2) / stupid; // 上舵机
    int b = (b1 - b2) / stupid; // 下舵机
    if (a < 0 && pos_1 > upmin)
    {
        pos_1 -= step;
        if (pos_1 < upmin)
            pos_1 = upmin;
    }
    else if (a > 0 && pos_1 < upmax)
    {
        pos_1 += step;
        if (pos_1 > upmax)
            pos_1 = upmax;
    }
    if (pos_1 <= 90) // 上舵机角度<90°时
        b = -b;
    if (b < 0 && pos_2 < downmax)
    {
        pos_2 += step;
        if (pos_2 > downmax)
            pos_2 = downmax;
    }
    else if (b > 0 && pos_2 > downmin)
    {
        pos_2 -= step;
        if (pos_2 < downmin)
            pos_2 = downmin;
    }

    servo_1.write(pos_1);
    servo_2.write(pos_2);
}

void TIME(Time nowtime)
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Time: ");
    lcd.print(nowtime.hr);
    lcd.print(":");
    lcd.print(nowtime.min);

    lcd.setCursor(0, 1);
    lcd.print("Date: ");
    lcd.print(nowtime.yr);
    lcd.print("/");
    lcd.print(nowtime.mon);
    lcd.print("/");
    lcd.print(nowtime.date);
}

void DHT()
{
    int t = (int)dht.readTemperature();
    int h = (int)dht.readHumidity();
    lcd.clear();
    Serial.print("Tempereture:");
    Serial.print(t);
    Serial.println("℃");
    Serial.print("Humidity:");
    Serial.print(h);
    Serial.println("%RH");

    lcd.setCursor(0, 0);
    lcd.print("Tem:");
    lcd.print(t);
    lcd.print("'C");
    lcd.setCursor(0, 1);
    lcd.print("Hum:");
    lcd.print(h);
    lcd.print("%RH");
}

void CLEAN()
{
    pos_1 = 170;
    pos_2 = 10;
    servo_1.write(pos_1);
    servo_2.write(pos_2);
    digitalWrite(waterPIN, HIGH);
    delay(1000); // 清洁时间
    digitalWrite(waterPIN, LOW);
    servo_3.write(cleanStartAngle);
    delay(500);
    servo_3.write(cleanEndAngle);
    delay(500);
    servo_3.write(cleanStartAngle);
    delay(500);
    servo_3.write(cleanEndAngle);
    delay(500);
    servo_3.write(cleanStartAngle);
    delay(500);
    servo_3.write(cleanEndAngle);
    delay(500);
    servo_3.write(cleanStartAngle);
    delay(1000);
}