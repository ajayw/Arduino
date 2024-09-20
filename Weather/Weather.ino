//申请注册获取天气API网址：https://openweathermap.org/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// Wi-Fi 连接信息
const char* ssid = "ZHIYI";
const char* password = "zy12345678";
int number = 0;

// 设置 NTP 客户端获取时间
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 28800, 60000); // UTC+8 时区（中国）

// OpenWeatherMap API 信息
const char* apiKey = "25f972585aadf21018e53c6161c86b07";  // 使用你的 API 密钥
const char* city = "Shenzhen";       // 使用你的城市名称
const char* server = "http://api.openweathermap.org/data/2.5/weather?q=";

// LCD 初始化
LiquidCrystal_I2C lcd(0x27, 16, 2); // 修改 I2C 地址如有需要

// 自定义字符数据（5x8 像素）
byte sun[8] = {
  0b00100,
  0b10101,
  0b01110,
  0b11111,
  0b01110,
  0b10101,
  0b00100,
  0b00000
};

// 替换后的图标数组
byte fewClouds[8] = {
  0b00000,
  0b00111,
  0b01111,
  0b11100,
  0b11111,
  0b01110,
  0b00000,
  0b00000
};

byte scatteredClouds[8] = {
  0b00000,
  0b00000,
  0b01110,
  0b11111,
  0b11111,
  0b01110,
  0b00000,
  0b00000
};

byte brokenClouds[8] = {
  0b00111,
  0b01111,
  0b11100,
  0b11111,
  0b01110,
  0b00000,
  0b00000,
  0b00000
};

byte showerRain[8] = {
  0b00000,
  0b01110,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b10100,
  0b00101
};

byte rain[8] = {
  0b00000,
  0b01110,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b10010,
  0b01001
};

byte thunderstorm[8] = {
  0b00000,
  0b01110,
  0b11111,
  0b11111,
  0b00100,
  0b01010,
  0b10001,
  0b00100
};

byte snow[8] = {
  0b00100,
  0b01010,
  0b00100,
  0b11111,
  0b11111,
  0b00100,
  0b01010,
  0b00100
};

byte mist[8] = {
  0b00000,
  0b11011,
  0b00000,
  0b11111,
  0b00000,
  0b11011,
  0b00000,
  0b00000
};

byte clearNight[8] = {
  0b00010,
  0b00100,
  0b01000,
  0b10000,
  0b01000,
  0b00100,
  0b00010,
  0b00000
};

byte fewCloudsNight[8] = {
  0b00000,
  0b00111,
  0b01111,
  0b11100,
  0b11111,
  0b01110,
  0b00000,
  0b00000
};

byte scatteredCloudsNight[8] = {
  0b00000,
  0b00000,
  0b01110,
  0b11111,
  0b11111,
  0b01110,
  0b00000,
  0b00000
};

byte brokenCloudsNight[8] = {
  0b00111,
  0b01111,
  0b11100,
  0b11111,
  0b01110,
  0b00000,
  0b00000,
  0b00000
};

byte showerRainNight[8] = {
  0b00000,
  0b01110,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b10100,
  0b00101
};

byte rainNight[8] = {
  0b00000,
  0b01110,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b10010,
  0b01001
};

byte thunderstormNight[8] = {
  0b00000,
  0b01110,
  0b11111,
  0b11111,
  0b00100,
  0b01010,
  0b10001,
  0b00100
};

void setup() {
  Serial.begin(115200);
  Wire.begin(33, 32);  // 使用 SDA (33) 和 SCL (32)

  // LCD 初始化
  lcd.init();
  lcd.backlight();
  
  // 创建自定义字符
  lcd.createChar(0, sun);                // 索引 0：晴天白天 (01d)
  lcd.createChar(1, fewClouds);          // 索引 1：少云白天 (02d)
  lcd.createChar(2, scatteredClouds);    // 索引 2：多云白天 (03d)
  lcd.createChar(3, brokenClouds);       // 索引 3：阴天白天 (04d)
  lcd.createChar(4, showerRain);         // 索引 4：阵雨 (09d)
  lcd.createChar(5, rain);               // 索引 5：小雨 (10d)
  lcd.createChar(6, thunderstorm);       // 索引 6：雷暴 (11d)
  lcd.createChar(7, snow);               // 索引 7：雪 (13d)
  lcd.createChar(8, mist);               // 索引 8：雾 (50d)
  lcd.createChar(9, clearNight);         // 索引 9：晴天夜晚 (01n)
  lcd.createChar(10, fewCloudsNight);    // 索引 10：少云夜晚 (02n)
  lcd.createChar(11, scatteredCloudsNight); // 索引 11：多云夜晚 (03n)
  lcd.createChar(12, brokenCloudsNight); // 索引 12：阴天夜晚 (04n)
  lcd.createChar(13, showerRainNight);   // 索引 13：夜间阵雨 (09n)
  lcd.createChar(14, rainNight);         // 索引 14：夜间小雨 (10n)
  lcd.createChar(15, thunderstormNight); // 索引 15：夜间雷暴 (11n)

  // Wi-Fi 连接
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");

  // 初始化 NTP 客户端
  timeClient.begin();
  timeClient.update();

  // 显示连接状态
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connected!");
  delay(2000);
  lcd.clear();
  getWeather();
}

void loop() {

  // 更新时间
  timeClient.update();
  String formattedTime = timeClient.getFormattedTime();
  // 显示时间
  lcd.setCursor(0, 1);
  lcd.print("Time:");
  lcd.print(formattedTime);

  delay(1000);  // 每 10 分钟更新一次天气
  number++;
  if (number > 60) {
    getWeather();
    Serial.println("tianqi");
    number = 0;
  }
}

void getWeather() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String(server) + city + "&appid=" + apiKey + "&units=metric";
    http.begin(url);
    int httpCode = http.GET();
    if (httpCode == 200) {
      String payload = http.getString();
      // Serial.println(payload); // 打印收到的 JSON 数据
      // 解析 JSON 数据
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);
        // 提取天气信息
        int temperature = doc["main"]["temp"];
        const char* icon = doc["weather"][0]["icon"];
        const char* description = doc["weather"][0]["description"];

        // 显示天气信息
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(String(temperature) + "C");
        lcd.setCursor(5, 0);
        if (String(icon) == "01d") {  // 晴天（白天）
          lcd.write(byte(0));  // 太阳图标
          lcd.print(" Clear");
        } else if (String(icon) == "01n") {  // 晴天（夜晚）
          lcd.write(byte(9));  // 月亮图标
          lcd.print(" Clear Night");
        } else if (String(icon) == "02d") {  // 少云（白天）
          lcd.write(byte(1));  // 少云图标
          lcd.print(" Few Clouds");
        } else if (String(icon) == "02n") {  // 少云（夜晚）
          lcd.write(byte(10));  // 少云夜晚图标
          lcd.print(" Few Clouds Night");
        } else if (String(icon) == "03d") {  // 多云（白天）
          lcd.write(byte(2));  // 多云图标
          lcd.print(" Scattered Clouds");
        } else if (String(icon) == "03n") {  // 多云（夜晚）
          lcd.write(byte(11));  // 多云夜晚图标
          lcd.print(" Scattered Clouds Night");
        } else if (String(icon) == "04d") {  // 阴天（白天）
          lcd.write(byte(3));  // 阴天图标
          lcd.print(" Broken Clouds");
        } else if (String(icon) == "04n") {  // 阴天（夜晚）
          lcd.write(byte(12));  // 阴天夜晚图标
          lcd.print(" Broken Clouds Night");
        } else if (String(icon) == "09d") {  // 阵雨（白天）
          lcd.write(byte(4));  // 阵雨图标
          lcd.print(" Shower Rain");
        } else if (String(icon) == "09n") {  // 阵雨（夜晚）
          lcd.write(byte(13));  // 夜间阵雨图标
          lcd.print(" Shower Rain Night");
        } else if (String(icon) == "10d") {  // 小雨（白天）
          lcd.write(byte(5));  // 小雨图标
          lcd.print(" Rain");
        } else if (String(icon) == "10n") {  // 小雨（夜晚）
          lcd.write(byte(14));  // 夜间小雨图标
          lcd.print(" Rain Night");
        } else if (String(icon) == "11d") {  // 雷暴（白天）
          lcd.write(byte(6));  // 雷暴图标
          lcd.print(" Thunderstorm");
        } else if (String(icon) == "11n") {  // 雷暴（夜晚）
          lcd.write(byte(15));  // 夜间雷暴图标
          lcd.print(" Thunderstorm Night");
        } else if (String(icon) == "13d" || String(icon) == "13n") {  // 雪
          lcd.write(byte(7));  // 雪图标
          lcd.print(" Snow");
        } else if (String(icon) == "50d" || String(icon) == "50n") {  // 雾
          lcd.write(byte(8));  // 雾图标
          lcd.print(" Mist/Fog");
        } else {
          lcd.print("Weather: " + String(description));
        }
    }
    http.end();
  }
}
