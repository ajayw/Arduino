/*
  Bluetooth smart home system ino v1.0.20
  Date: 2023.9.7
  Author: Ajay Huajian
  2023 Copyright(c) ZHIYI Technology Inc. All right reserved
*/
#include <Adafruit_NeoPixel.h>    //RGB libraries
#include <dht.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SPI.h> 
#include <RFID.h>
#include <Servo.h>                // Servo libraries

#define NUMPIXELS     4           //Number of LED lamp beads
#define RGB_PIN       5           //RGB module Arduino output control signal pin
#define LED_PIN       8           //LED module Arduino output control signal pin
#define touchKEY      A6          //Touch button Arduino output control signal pin A6
#define btnKEY        A7          //Yellow button module Arduino output control signal pin
#define buzzer        3           //Buzzer module Arduino output control signal pin
#define smokeA0       A0          //MQ-2 gas sensor Arduino output control signal pin
#define DHT11_PIN     4           //DHT11 temperature and humidity Arduino output control signal pin
#define RelayPin      16          //Relay module Arduino output control signal pin    
#define FanPinA       7           //Fan motor module Arduino output control signal pin
#define rain_Pin      A3          //Raindrop sensor Arduino output control signal pin

//Variable definitions
char ser_val;               //蓝牙数据变量    Bluetooth data variable
int count_key=5;            //RGB灯切换序号   RGB light switch serial number
int Press=0;                //Defines key flag bits
int KEY_BIT=0;              //Defining status bits
bool LED_BIT=false;         //Defining LED status
int setSmokeVal = 50;       //Set the gas concentration range 50
bool RelayMode = true;      //风扇控制模式：默认自动感应烟雾启动风扇模式，当手动关闭风扇时自动感应烟雾依然有效，当手动打开风扇时不需感应烟雾。
bool Relay_switch = false;  //风扇继电器开关默认关 Relay switch State transitions
bool RelayBtn = false;      //APP风扇按钮开关状态
bool windowStatus= true;    //窗户开关状态 true：开窗；false:关窗
bool windowFlag = true;     //false：手动控制窗户；true：自动控制窗户
bool LCD_switch = false;
bool doorCheck= false;      //门验证状态；false:Access Denied ;true:Access Granted
bool lockStatus = true;     //门锁状态；false:unlock ; true:lock
int lockDoor = 10;          //Lock position limitation
int openDoor =90;           //Limit of unlock position
int winOpenPos = 10;        //窗户打开的舵机角度    The steering Angle at which the window opens
int winClosePos =130;       //窗户关闭的舵机角度    The steering Angle at which the window closes
int RainValue;              //雨滴传感器数值    Raindrop sensor values
int touchValue;             //触控按钮数值    Touch button values
struct SensorData {         //定义温湿度结构体变量    Define the temperature and humidity structure variables
    float temperature;
    float humidity;
};
String dataString;

RFID rfid(10, 9);                 //D10: the pin of the tag reader SDA. D9: Label reader RST pin
unsigned char str[MAX_LEN];       //MAX_LEN is 16: size of the array 
String accessGranted [2] = {"210101368113", "96111131033"};   //Rfid serial number authorized access
int accessGrantedSize = 2;                                    //Number of serial numbers
Servo doorServo;          //门舵机实例对象    Locking mechanism servo
Servo windowServo;        //窗户舵机实例对象    Create a servo object to control the servo
dht DHT;                  //温湿度传感器实例对象      

//Set the LCD address to 0x27 to display in 16 characters and 2 lines
LiquidCrystal_I2C lcd(0x27,20,4);

Adafruit_NeoPixel pixels(NUMPIXELS, RGB_PIN, NEO_GRB + NEO_KHZ800);//Create light objects

void setup() {
  // put your setup code here, to run once:
	Serial.begin(9600);
  pinMode(touchKEY,INPUT);        //Touch definition input
	pinMode(btnKEY,INPUT);          //Key definition input
  pinMode(LED_PIN,OUTPUT);        //LED
  pinMode(buzzer, OUTPUT);        //The buzzer defines the output
  pinMode(smokeA0, INPUT);        //Smoke definition input
  pinMode(FanPinA, OUTPUT);       //fan
  pinMode(RelayPin,OUTPUT);       //Relay module
  pinMode(DHT11_PIN, INPUT);      //设置温湿度模块为输入
  pinMode(RGB_PIN,OUTPUT);        //设置RGB灯模块为输出
  windowServo.attach(6);          // Connect the servo on pin 6 to the servo object
  windowServo.write(winOpenPos); //初始设置窗户为关闭
  pinMode(rain_Pin, INPUT);       //初始设置雨滴传感器为输入
  digitalWrite(RelayPin, LOW);    //设置继电器为低电平断开
  digitalWrite(FanPinA, LOW);     //设置风扇为低电平关闭
  SPI.begin();                    //Start SPI communication with the reader
  lcd.init();                     //Initialized liquid crystal
  lcd.noBacklight();              //初始关闭LCD屏背光
	pixels.begin();                 //Initialize 2812 library functions
  pixels.clear();
	pixels.show();
  rfid.init();                    //初始化门禁感应器  initialize 
  doorServo.attach(15);           //pin A1
  doorServo.write(lockDoor);      //初始设置门舵机为锁上状态    Move the servo to the locked position
  // Serial.println("DHT TEST PROGRAM ");
  // Serial.print("LIBRARY VERSION: ");
  // Serial.println(DHT_LIB_VERSION);
  // Serial.println();
  // Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)");
}

void loop() {
  TouchKey();         //触控按钮函数  The touch key function
	RGB();              //RGB灯控制   RGB lamp control
  BluetoothData();    //蓝牙控制逻辑    Bluetooth Control logic
  LED();              //LED控制   LED control
  windowControl();    //窗户控制    Window control
  RFID();             //门禁RFID    RFID for access control
  smoke();            //烟雾警报函数    Smoke alarm function
  sendSensorData();   //蓝牙串口发送温湿度数据函数    Bluetooth serial port to send temperature and humidity data function
  ShowHumiture();     //温湿度LCD显示函数   Temperature and humidity LCD display function
}

/*
*接收蓝牙数据函数 BluetoothData()
*
*/
void BluetoothData(){
  if(Serial.available() > 0) {  //判断收到蓝牙数据    Verify receipt of Bluetooth data
    ser_val = Serial.read();
    // Serial.println(ser_val);
    switch(ser_val){                            //判断接收到LCD开关按钮被点击     Determine that the received LCD switch button is clicked
      case 'A':count_key = 4;break;             //RGB亮白灯   RGB bright white light
      case 'B':count_key = 1;break;             //RGB亮红灯   RGB lights up red
      case 'C':count_key = 2;break;             //RGB亮绿灯   RGB light green
      case 'D':count_key = 3;break;             //RGB亮蓝灯   RGB light blue light
      case 'E':count_key = 5;break;             //RGB灭灯   RGB out light
      case 'F':LCD_switch = !LCD_switch;break;  //LCD温湿度开关状态转换     LCD Temperature and humidity switch State transitions
      case 'G':               //风扇控制模式转换     fan control mode transitons
          if(!RelayMode){     //自动感应烟雾报警启动风扇比手动控制风扇优先级高，只有在无烟雾超标警报情况下才能手动控制风扇开关。
              RelayBtn = true;
              Relay_switch = !Relay_switch;     //风扇打开或关闭    The fan is turned on or off
          }else{
              RelayBtn = false;
          }
      break;    
      case 'H':doorControl();break;             //门开关状态    Door opening state
      case 'I':windowFlag = false;windowStatus = !windowStatus;break;   //windowFlag = false切换成手动模式，接收到I后取反切换
      case 'J':windowFlag = true;break;         //窗户控制方式切换为自动    Window control mode is switched to automatic
      case 'K':LED_BIT = !LED_BIT;break;        //LED按钮切换状态   LED button switch state
      default: break;
    }
  }
}

//The touch key initializes the function
void TouchKey()   //触控按钮函数   Touch Key Function
{
  touchValue=analogRead(touchKEY);    //A6引脚使用读取模拟方法获取触摸模块数值    Pin A6 uses the read simulation method to obtain the touch module value
	if((touchValue<300)&&(Press==0)){  
    	Press=1;                //按下标志==1   Press the sign
	}
  touchValue=analogRead(touchKEY);
  if((touchValue>1000) &&(Press==1))    //松开触控按钮且按下标志==1   Let go detection
  {
    Press=0;
    count_key++;
    if(count_key>5)count_key=1;         //超过5次回到初始值1   Return to the initial value of 1 more than 5 times
    // Serial.print("count_key:");
    // Serial.println(count_key);
  }
    // Serial.print("touchValue:");
    // Serial.println(touchValue);
}

//LED function
void LED()
{
  if((analogRead(btnKEY))&&(KEY_BIT)) //Determine if the key is pressed
  {
    KEY_BIT=0;
    LED_BIT=!LED_BIT;
  }else{
    delay(20);
    if(!analogRead(btnKEY)){          //判断按钮被按下时将KEY_BIT置1，防止抖动重复执行LED_BIT取反.
      KEY_BIT=1;
    }
  }
  if(LED_BIT)digitalWrite(LED_PIN,HIGH);  //MCU output high level
  else digitalWrite(LED_PIN,LOW);         //MCU output low level
}

//RGB function
void RGB()
{
  switch(count_key)       //Number of keys 
  {
    case 1 :              //The red light is on.
      pixels.setPixelColor(0, pixels.Color(180, 0, 0));
      pixels.setPixelColor(1, pixels.Color(180, 0, 0));
      pixels.setPixelColor(2, pixels.Color(180, 0, 0));
      pixels.setPixelColor(3, pixels.Color(180, 0, 0));
      pixels.show();
      break;
    case 2 :              //green light flashes
      pixels.setPixelColor(0, pixels.Color(0, 180, 0));
      pixels.setPixelColor(1, pixels.Color(0, 180, 0));
      pixels.setPixelColor(2, pixels.Color(0, 180, 0));
      pixels.setPixelColor(3, pixels.Color(0, 180, 0));
      pixels.show();
      break;
    case 3 :               //The blue lights
      pixels.setPixelColor(0, pixels.Color(0, 0, 180));
      pixels.setPixelColor(1, pixels.Color(0, 0, 180));
      pixels.setPixelColor(2, pixels.Color(0, 0, 180));
      pixels.setPixelColor(3, pixels.Color(0, 0, 180));
      pixels.show();
      break;
    case 4 :           //bright white light
      pixels.setPixelColor(0, pixels.Color(180, 180, 180));
      pixels.setPixelColor(1, pixels.Color(180, 180, 180));
      pixels.setPixelColor(2, pixels.Color(180, 180, 180));
      pixels.setPixelColor(3, pixels.Color(180, 180, 180));
      pixels.show();
      break;
    case 5 :        // Turn our the lights
      pixels.setPixelColor(0, pixels.Color(0, 0, 0));
      pixels.setPixelColor(1, pixels.Color(0, 0, 0));
      pixels.setPixelColor(2, pixels.Color(0, 0, 0));
      pixels.setPixelColor(3, pixels.Color(0, 0, 0));
      pixels.show();
      break;
     default : break;
  }
}

/*
*窗户状态函数 windowControl()
*自动模式下雨滴传感器状态影响窗户开关
*手动模式下同一按钮打开或关闭窗户
*/
void windowControl()
 {
    // Serial.print("RainValue =");
    // Serial.println(RainValue);
    if(windowFlag){                         //windowFlag为“真”，即窗户在自动模式下
        RainValue = analogRead(rain_Pin);   //获取雨滴传感器的值保存到RainValue
        if(RainValue < 800){
          windowServo.write(winClosePos);   //close the door
          windowStatus = false;
        }else{
          windowServo.write(winOpenPos);    //open the door
          windowStatus = true;
        }
    }else{                                  //否则为手动模式，接收到I后取反切换
        if(windowStatus){
          windowServo.write(winOpenPos);    //open the door
        }else{
          windowServo.write(winClosePos);   //close the door
        }
    }
}

//门禁检测是否有卡RFID
void RFID()
{
  if (rfid.findCard(PICC_REQIDL, str) == MI_OK)   //Wait for a tag to be placed near the reader
  {
    Serial.println("Card found");
    String temp = "";                             //A temporary variable that stores the READ RFID number
    if (rfid.anticoll(str) == MI_OK)              //Anti-collision detection, read the label serial number
    {
      Serial.print("The card's ID number is : ");
      for (int i = 0; i < 4; i++)                 //Record and display the label serial number
      {
        temp = temp + (0x0F & (str[i] >> 4));
        temp = temp + (0x0F & str[i]);
      }
      Serial.println (temp);
      checkAccess (temp);     //Check whether the identified label is allowed to be opened
    }
    rfid.selectTag(str); //Lock the card to prevent repeated reading, removing the line will cause the sketch to read the card continuously
  }
  rfid.halt();
}
//门禁RFID验证函数    
void checkAccess (String temp)    //The function checks whether a registered tag is accessible
{
  // doorCheck = false;
  for (int i=0; i <= (accessGrantedSize-1); i++)    //Iterates over all tag ID numbers registered in the group
  {
    if(accessGranted[i] == temp)            //If a label is found, then open/close the lock
    {
      // Serial.println ("Access Granted");
      // doorCheck = true;
      doorControl();                      //调用开关门函数
    }
  }
  // if (doorCheck == false)     //If no tags are found
  // {
  //   // Serial.println ("Access Denied");
  // }
}
//门控制函数
void doorControl(){
    if (lockStatus){         //If the lock is closed, open it
        doorServo.write(openDoor);
        lockStatus = false;
    }else{  //If the lock is open, close it
        doorServo.write(lockDoor);
        lockStatus = true;
    }
}

//Mq-2 gas function
void smoke()
{
  int smokeValue = analogRead(smokeA0); //Assign the check value of the MQ-2 sensor to the ALOG sensor variable

  // Serial.print("Pin A0: ");
  // Serial.println(smokeValue);
  //Check if the threshold has been reached
    if (smokeValue > setSmokeVal)
    {
      tone(buzzer, 1000, 200);              //蜂鸣器响    The buzzer sounded
      Relay_switch = true;
      RelayMode = true;
      digitalWrite(RelayPin,HIGH);  //High output; Relay start
    }else if(!RelayBtn){
      noTone(buzzer);                       //蜂鸣器停    The buzzer stops
      Relay_switch = false;
      RelayMode = false;
    }
    RelayControl();
}

//继电器及风扇控制函数
void RelayControl(){
  if(Relay_switch){
    digitalWrite(RelayPin,HIGH);  //High output; Relay start
    digitalWrite(FanPinA, HIGH);
  }else{
    digitalWrite(RelayPin,LOW);
    digitalWrite(FanPinA, LOW);
  }
}

/*
*蓝牙串口发送温湿度数据函数 sendSensorData()
*以字节方式发送
*/
void sendSensorData() {
    int chk = DHT.read11(DHT11_PIN);

    SensorData data = { DHT.temperature, DHT.humidity };    //采样温度和湿度数据   Sample temperature and humidity data
    delay(100);
    dataString = String(data.temperature, 1) + "," + String(data.humidity, 1);   //保留一位小数点将温湿度以逗号隔开方式拼接成字符串  Convert temperature and humidity to string
    if(LCD_switch){   //打开开关正常显示温湿度,否则APP上温湿度不再更新。如果不想受到按钮影响始终显示在APP就去掉if……。
      Serial.println(dataString);       //发送温湿度数据     Send the data string
    }
}

//温湿度LCD显示函数   Display temperature
void ShowHumiture()
{
   if(LCD_switch){
      lcd.backlight();            //打开LCD背光 Open the backlight
      lcd.setCursor(0,0);
      lcd.print("humi:");
      lcd.setCursor(5,0);
      lcd.print(DHT.temperature,1); //LCD屏显示温度值   The LCD screen displays the temperature value

      lcd.setCursor(0,1);
      lcd.print("temp:");
      lcd.setCursor(5,1);
      lcd.print(DHT.humidity,1);    //LCD屏显示湿度值   The LCD screen displays the humidity value
    }else{
      lcd.noBacklight();          //关闭LCD   Close the backlight
      lcd.clear();                //清除内容    clear LCD
    }
}

