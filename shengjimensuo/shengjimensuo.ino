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
int blueLED = 27;    // 新增蓝色LED 
int servoPin = 18;
int pirPin = 33;      // PIR传感器引脚 
int speakerPin = 34;  // ISD1820扬声器引脚

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
bool isEnteringPassword = false;  // 是否在输入密码过程中

unsigned long lastActivityTime = 0;  // 记录上次操作的时间
const unsigned long idleTimeout = 5000;  // 5秒超时
unsigned long pirLastDetected = 0;  // 记录PIR检测到的最后时间
const unsigned long pirResetTimeout = 10000; // PIR检测10秒后复位

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
  unsigned long currentTime = millis();
  
  // 检测PIR传感器
  if (digitalRead(pirPin) == HIGH && !pirDetected && !isEnteringPassword) {
    pirDetected = true; // 确保只触发一次
    pirLastDetected = currentTime;  // 记录PIR触发时间
    lastActivityTime = currentTime;  // 记录活动时间
    digitalWrite(blueLED, HIGH);     // 人体感应时蓝灯亮起
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("请入密码");
    display.display();
    playVoice("请输入密码");  // 语音提示

  } else if (digitalRead(pirPin) == LOW && (currentTime - pirLastDetected > pirResetTimeout)) {
    pirDetected = false;   // PIR传感器复位，允许再次触发
    digitalWrite(blueLED, LOW);  // 关闭蓝灯
  }

  // 处理密码输入
  char key = keypad.getKey();
  if (key && inputPassword.length() < passwordLength) {  
    lastActivityTime = currentTime;  // 记录每次按键的时间
    isEnteringPassword = true;       // 设置正在输入密码状态
    playSound();  // 播放按键音
    
    inputPassword += key;
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("输入: ");
    display.print(inputPassword);
    display.display();

    // 当输入密码达到设定长度
    if (inputPassword.length() == passwordLength) {
      if (inputPassword == correctPassword) {
        unlock();  // 密码正确解锁
      } else {
        passwordError();  // 密码错误处理
      }
      inputPassword = "";  // 重置输入
      isEnteringPassword = false;  // 重置输入状态
    }
  }

  // 检查是否达到无操作超时
  if (currentTime - lastActivityTime > idleTimeout && !isEnteringPassword) {
    resetDisplay();  // 超时后恢复默认界面
  }
}

// 密码正确时解锁
void unlock() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("欢迎回家");
  display.display();
  digitalWrite(greenLED, HIGH);
  playVoice("欢迎回家");  // 播放欢迎语音

  myServo.write(90);  // 舵机旋转解锁
  delay(3000);  // 等待3秒
  myServo.write(0);  // 复位锁定
  digitalWrite(greenLED, LOW);
}

// 密码错误时提示
void passwordError() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("密码错误");
  display.display();
  digitalWrite(redLED, HIGH);
  playVoice("密码错误");  // 播放密码错误语音
  delay(1000);
  digitalWrite(redLED, LOW);
}

// 播放ISD1820声音
void playVoice(String phrase) {
  if (phrase == "请输入密码") {
    // 执行语音提示 "请输入密码" 的播放逻辑
  } else if (phrase == "密码错误") {
    // 执行语音提示 "密码错误" 的播放逻辑
  } else if (phrase == "欢迎回家") {
    // 执行语音提示 "欢迎回家" 的播放逻辑
  }
  // 模拟播放语音
  digitalWrite(speakerPin, HIGH);
  delay(1000);  // 模拟语音播放的时间
  digitalWrite(speakerPin, LOW);
}

// 播放按键声音
void playSound() {
  digitalWrite(speakerPin, HIGH);
  delay(200);  // 模拟按键音
  digitalWrite(speakerPin, LOW);
}

// 屏幕重置为初始状态
void resetDisplay() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.print("请输入密码");
  display.display();
  inputPassword = "";  // 重置密码输入
  pirDetected = false; // 重置PIR传感器状态
  digitalWrite(blueLED, LOW);  // 关闭蓝灯
}