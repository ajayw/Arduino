#include <Wire.h>
#include <Adafruit_SSD1306.h>  // OLED库
#include <Keypad.h>            // 键盘库
#include <ESP32Servo.h>        // 舵机库

// OLED屏幕设置
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// 设置舵机、LED、传感器和扬声器引脚
Servo myServo;
int redLED = 25;
int greenLED = 26;
int blueLED = 0;    // 新增蓝色LED
int servoPin = 18;
int pirPin = 4;      // PIR传感器引脚
int speakerPin = 27; // ISD1820扬声器引脚
int BuzzerPin = 2;
// 键盘设置
const byte ROWS = 4; 
const byte COLS = 4; 
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {32, 19, 23, 5}; 
byte colPins[COLS] = {13, 12, 14, 15}; 
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// 密码设置
String correctPassword = "1397";  // 设置4位密码
String inputPassword = "";
int passwordLength = 4;
bool pirDetected = false;  // 标志位，确保PIR传感器只触发一次

unsigned long lastActivityTime = 0;  // 记录上次操作的时间
const unsigned long idleTimeout = 4000;  // 4秒超时

void setup() {
  // 初始化串口
  Serial.begin(115200);

  // 初始化OLED屏幕
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // 使用 0x3C 作为 I2C 地址
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  resetDisplay();  // 初始显示 "请输入密码"

  // 初始化舵机和LED
  myServo.attach(servoPin, 500, 2400);
  myServo.write(0);  // 初始状态为锁定
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(blueLED, OUTPUT);    // 初始化蓝色LED
  pinMode(speakerPin, OUTPUT); // 初始化ISD1820扬声器
  pinMode(pirPin, INPUT);      // 初始化PIR传感器
}

void loop() {
  // 检测PIR传感器
  if (digitalRead(pirPin) == HIGH && !pirDetected&&(inputPassword.length()<=0)) {
    pirDetected = true; // 确保只触发一次
    lastActivityTime = millis();  // 记录PIR触发时间
    digitalWrite(blueLED, HIGH); // 人体感应时蓝灯亮起
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Enter password:");
    display.display();

    // 语音提示 "请输入密码"
    digitalWrite(BuzzerPin, HIGH); // 模拟ISD1820播放语音
    delay(1000);                    // 播放语音时间
    digitalWrite(BuzzerPin, LOW);
  }

  // 处理密码输入
  char key = keypad.getKey();
  if (key && inputPassword.length() < passwordLength) {  // 防止密码过长
    lastActivityTime = millis();  // 记录每次按键的时间
    analogWrite(BuzzerPin, 250); // 每次按键扬声器播放按键音
    delay(200);
    analogWrite(BuzzerPin, 0);

    inputPassword += key;
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Input: ");
    display.print(inputPassword);
    display.display();

    // 当输入密码达到设定长度
    if (inputPassword.length() == passwordLength) {
      if (inputPassword == correctPassword) {
        // 密码正确
        display.clearDisplay();
        display.setCursor(0, 0);
        display.print("Welcome Home!");
        display.display();

        digitalWrite(greenLED, HIGH);
        digitalWrite(speakerPin, HIGH);  // 扬声器播放 "密码正确"
        // digitalWrite(speakerPin, LOW);
        // delay(500);
        // digitalWrite(speakerPin, HIGH);
        // delay(200);
        // digitalWrite(speakerPin, LOW);

        // 舵机旋转解锁
        myServo.write(90);
        delay(3000);  // 等待3秒
        myServo.write(0);  // 复位锁定
        digitalWrite(greenLED, LOW);

      } else {
        // 密码错误
        display.clearDisplay();
        display.setCursor(0, 0);
        display.print("Wrong! Retry");
        display.display();

        digitalWrite(redLED, HIGH);
        digitalWrite(BuzzerPin, HIGH);  // 扬声器播放 "密码错误"
        delay(1000);
        digitalWrite(BuzzerPin, LOW);
        digitalWrite(redLED, LOW);
      }
      inputPassword = "";  // 重置输入
    }
  }

  // 检查是否达到无操作超时
  if (millis() - lastActivityTime > idleTimeout) {
    resetDisplay();  // 超时后恢复默认界面
  }
}

// 重置显示为初始状态
void resetDisplay() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.print("Enter password");
  display.display();
  inputPassword = "";  // 重置密码输入
  pirDetected = false; // 重置PIR传感器触发状态
  digitalWrite(blueLED, LOW);  // 关闭蓝色LED
}