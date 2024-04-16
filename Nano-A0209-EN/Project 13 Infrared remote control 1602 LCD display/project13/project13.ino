#include <Wire.h> 
#include <dht.h>                //添加温湿度库    Add temperature and humidity library
#include <LiquidCrystal_I2C.h>  //添加LCD显示屏库   Add LCD display library
#include <IRremote.h>           //添加红外接收器库    Reference infrared library

dht DHT;              //实例化温湿度对象为DHT   Instantiate the temperature and humidity object as a DHT

#define DHT11_PIN 4   //定义温湿度传感器引脚4   Define pin 4 of the temperature and humidity sensor
#define IR_PIN 2      //定义红外接收器引脚2   Define pin 2 of the infrared receiver

#define ON  0XFFA25D  //遥控器按钮1的值为 0XFFA25D   The value of button 1 of the remote control is 0XFFA25D
#define OFF 0XFF629D  //遥控器按钮2的值为 0XFF629D    The value of button 2 of the remote control is 0XFF629D

//设置LCD地址为0x27，以实现16字符和2行显示   set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27,20,4);  
IRrecv irrecv(IR_PIN);  //实例化红外接收器对象为irrecv    Instantiate the infrared receiver object as irrecv
decode_results results; //红外接收数据指针    Infrared receiving data pointer

void setup()
{
  Serial.begin(115200);
  lcd.init();           //初始化lcd   initialize the lcd 
  lcd.backlight();      //开启LCD背光   Enable LCD backlight
  irrecv.enableIRIn();  //启动红外接收    Start the receiver
  // Print a message to the LCD.
  // Serial.println("DHT TEST PROGRAM ");
  // Serial.print("LIBRARY VERSION: ");
  // Serial.println(DHT_LIB_VERSION);
  // Serial.println();
  // Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)");
}

void loop()
{
    ShowHumiture();               //调用温湿度显示函数    Call the temperature and humidity display function
    if (irrecv.decode(&results))  //判断接受到红外遥控信息    Determine received infrared remote control information
    {
        Serial.print("value = ");
        Serial.println(results.value, HEX);//十六进制方式打印信息值   Print information values in hexadecimal
        if (results.value == ON ) //接收到红外编码值与NO变量一致    The received IR coded values are consistent with the NO variable
        {
          lcd.backlight();        //开启LCD背光   Open the backlight
        } 
        else if (results.value == OFF ) //接收到红外编码值与OFF变量一致   The received IR coded values are consistent with the OFF variable
        {
            lcd.noBacklight();    //关闭LCD背光   Close the backlight
            lcd.clear();
        } 
        irrecv.resume();          //接收下一个值  Receive the next value
  }
  delay(100);
}

void ShowHumiture()
{
    int chk = DHT.read11(DHT11_PIN);
    //Serial.println("DHT TEST PROGRAM ");
    lcd.setCursor(0,0);
    lcd.print("temp:");
    lcd.setCursor(5,0);
    lcd.print(DHT.temperature);//在第一行显示温度值    Display the temperature value

    lcd.setCursor(0,1);
    lcd.print("humi:");
    lcd.setCursor(5,1);
    lcd.print(DHT.humidity);  //在第二行显示湿度值    Display humidity value
}
