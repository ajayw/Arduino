#include <Wire.h>
#include <Adafruit_SSD1306.h>  // OLED库
#include <Keypad.h>            // 键盘库
#include <ESP32Servo.h>        // 舵机库

// OLED屏幕设置
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// 设置舵机和LED引脚
Servo myServo;
int redLED = 25;
int greenLED = 26;
int buzzer = 27;
int servoPin = 18;

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

void setup() {
  // 初始化串口
  Serial.begin(115200);

  // 初始化OLED屏幕
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // 使用 0x3C 作为 I2C 地址
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.print("Lock Ready");
  display.display();
  // 初始化舵机和LED
  myServo.attach(servoPin,500,2400);
  myServo.write(0);  // 初始状态为锁定
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(buzzer, OUTPUT);
}

void loop() {
  char key = keypad.getKey();
  
  if (key) {
    digitalWrite(buzzer,HIGH);  // 每次按键蜂鸣器响
    delay(200);
    digitalWrite(buzzer,LOW);  // 每次按键蜂鸣器响
    inputPassword += key;
    display.clearDisplay();
    display.setCursor(0,0);
    display.print("Input: ");
    display.print(inputPassword);
    display.display();
    
    // 当输入密码达到设定长度
    if (inputPassword.length() == passwordLength) {
      if (inputPassword == correctPassword) {
        // 密码正确
        display.clearDisplay();
        display.setCursor(0,0);
        display.print("Welcome Home!");
        display.display();
        digitalWrite(greenLED, HIGH);
        digitalWrite(buzzer,HIGH);  // 蜂鸣器响两声
        delay(200);
        digitalWrite(buzzer,LOW);
        delay(500);
        digitalWrite(buzzer,HIGH);  // 蜂鸣器响两声
        delay(200);
        digitalWrite(buzzer,LOW);
        
        // 舵机旋转解锁
        myServo.write(90);
        delay(3000);  // 等待3秒
        myServo.write(0);  // 复位锁定
        digitalWrite(greenLED, LOW);
      } else {
        // 密码错误
        display.clearDisplay();
        display.setCursor(0,0);
        display.print("Wrong! Retry");
        display.display();
        digitalWrite(redLED, HIGH);
        digitalWrite(buzzer,HIGH);  // 蜂鸣器长响1秒
        delay(1000);
        digitalWrite(buzzer,LOW);  // 蜂鸣器长响1秒
        digitalWrite(redLED, LOW);
      }
      inputPassword = "";  // 重置输入
    }
  }
}