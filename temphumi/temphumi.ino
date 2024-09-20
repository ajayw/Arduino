#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// 配置 Wi-Fi 信息
const char* ssid = "Xiaomi 14";     // Wi-Fi 名称
const char* password = "zhanghao@."; // Wi-Fi 密码

// 设置 DHT 传感器
#define DHTPIN 4           // DHT 数据引脚
#define DHTTYPE DHT11      // DHT 传感器类型 (DHT11 或 DHT22)
DHT dht(DHTPIN, DHTTYPE);

// 设置 LCD 显示屏 (0x27 是 I2C 地址，可根据实际情况调整)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// 设置 NTP 客户端获取时间
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 28800, 60000); // UTC+8 时区（中国）

void setup() {
  // 初始化串口
  Serial.begin(115200);
  Wire.begin(33, 32);
  // 初始化 LCD
  lcd.init();
  lcd.backlight();

  // 初始化 DHT 传感器
  dht.begin();

  // 连接 Wi-Fi
  WiFi.begin(ssid, password);
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    lcd.print(".");
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connected");

  // 初始化 NTP 客户端
  timeClient.begin();
  timeClient.update();
  delay(2000);
}

void loop() {
  // 更新时间
  timeClient.update();
  String formattedTime = timeClient.getFormattedTime();

  // 读取温湿度
  int h = dht.readHumidity();
  int t = dht.readTemperature();

  // 检查是否读取失败
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // 显示时间
  // lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  lcd.setCursor(5,0);
  lcd.print(formattedTime);

  // 显示温度和湿度
  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.print(t);
  lcd.print("C H:");
  lcd.print(h);
  lcd.print("%");
  Serial.println(t);
  Serial.println(formattedTime);
  delay(1000); // 每2秒更新一次
}