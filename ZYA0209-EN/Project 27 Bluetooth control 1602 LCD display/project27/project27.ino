#include <LiquidCrystal_I2C.h>//添加LCD屏库   Add LCD screen library
#include <Wire.h> 
#include <dht.h>      //添加温湿度库    Add temperature and humidity library
#define DHT11_PIN 4   //定义温湿度传感器引脚4   Define temperature and humidity sensor pin4
dht DHT;              //实例化温湿度对象为DHT   Instantiate the temperature and humidity object as a DHT
struct SensorData {   //定义温湿度结构体变量    Define the temperature and humidity structure variables
    float temperature;
    float humidity;
};
char ser_val;
bool LCD_switch = false;
//将16个字符和2行显示的LCD地址设置为0x27 set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27,20,4);

void setup()
{
  Serial.begin(9600);
  pinMode(DHT11_PIN, INPUT);
  lcd.init();              //初始化LCD屏 initialize the lcd 
  lcd.noBacklight();          //关闭LCD屏背光 Turn off the LCD backlight.
}

void loop()
{
  if(Serial.available() > 0) {  //判断收到蓝牙数据    Verify receipt of Bluetooth data
    ser_val = Serial.read();
    if( ser_val == 'F'){        //判断接收到LCD开关按钮被点击   Determine that the received LCD switch button is clicked
      LCD_switch = !LCD_switch; //按钮状态转换 State transitions
    }
  }
  sendSensorData();     //调用发送温湿度数据函数    Call the function that sends the temperature and humidity data 
  ShowHumiture();       //调用温湿度LCD显示函数     Call the temperature and humidity display function
}
/*
*蓝牙串口发送温湿度数据函数 sendSensorData()
*以字节方式发送
*/
void sendSensorData() {
    String dataString;
    if(LCD_switch){   //打开开关正常显示温湿度，否则默认0.0,0.0
    int chk = DHT.read11(DHT11_PIN);
    delay(100);
    SensorData data = { DHT.temperature, DHT.humidity };    //采样温度和湿度数据   Sample temperature and humidity data
      dataString = String(data.temperature, 1) + "," + String(data.humidity, 1);   //保留一位小数点将温湿度以逗号隔开方式拼接成字符串  Convert temperature and humidity to string
    }else{
      dataString = "0.0,0.0";
    }
    Serial.println(dataString);       //发送温湿度数据     Send the data string
}

void ShowHumiture(){
    if(LCD_switch){
      lcd.backlight();            //打开LCD背光 Open the backlight
      lcd.setCursor(0,0);
      lcd.print("humi:");
      lcd.setCursor(5,0);
      lcd.print(DHT.temperature); //LCD屏显示温度值   The LCD screen displays the temperature value

      lcd.setCursor(0,1);
      lcd.print("temp:");
      lcd.setCursor(5,1);
      lcd.print(DHT.humidity);    //LCD屏显示湿度值   The LCD screen displays the humidity value
    }else{
      lcd.noBacklight();          //关闭LCD   Close the backlight
      lcd.clear();                //清除内容    clear LCD
    }
}
