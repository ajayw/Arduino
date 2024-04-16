#include <Wire.h> 
#include <dht.h>                //添加温湿度库    Add temperature and humidity library
#include <LiquidCrystal_I2C.h>  //添加LCD显示屏库   Add LCD display library

#define DHT11_PIN 4 //定义温湿度传感器引脚4   Define temperature and humidity sensor pin4

dht DHT;            //实例化温湿度对象为DHT   Instantiate the temperature and humidity object as a DHT

//设置LCD地址为0x27，以实现16字符和2行显示   set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27,20,4); 

void setup()
{
  Serial.begin(115200);
  lcd.init();       //初始化lcd   initialize the lcd 
  lcd.backlight();  //开启LCD背光
  // Print a message to the LCD.
  // Serial.println("DHT TEST PROGRAM ");
  // Serial.print("LIBRARY VERSION: ");
  // Serial.println(DHT_LIB_VERSION);
  // Serial.println();
  // Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)");
}

void loop()
{
  ShowHumiture(); //调用温湿度显示函数    Call the temperature and humidity display function
}
/*
*温湿度显示函数 ShowHumiture()
*lcd.setCursor(Columns ,rows);  Start at 0
*/
void ShowHumiture()             //函数实现过程    Function implementation process
{
    int chk = DHT.read11(DHT11_PIN);  
    //Serial.println("DHT TEST PROGRAM ");
    lcd.setCursor(0,0);
    lcd.print("temp:");
    lcd.setCursor(5,0);
    lcd.print(DHT.temperature); //在第一行显示温度值    Display the temperature value

    lcd.setCursor(0,1);
    lcd.print("humi:");
    lcd.setCursor(5,1);
    lcd.print(DHT.humidity);    //在第二行显示湿度值    Display humidity value

    Serial.print("temp:");
    Serial.println(DHT.temperature);
    Serial.print("humi:");
    Serial.println(DHT.humidity);
    delay(100);
}
