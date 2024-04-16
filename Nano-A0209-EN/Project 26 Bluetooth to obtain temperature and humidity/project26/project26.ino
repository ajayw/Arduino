#include <dht.h>      //添加温湿度库    Add temperature and humidity library
#define DHT11_PIN 4   //定义温湿度传感器引脚4   Define temperature and humidity sensor pin4

dht DHT;              //实例化温湿度对象为DHT   Instantiate the temperature and humidity object as a DHT
struct SensorData {   //定义温湿度结构体变量    Define the temperature and humidity structure variables
    float temperature;
    float humidity;
};

void setup()
{
  pinMode(DHT11_PIN, INPUT);
  Serial.begin(9600);
}

void loop()
{
  sendSensorData();     //调用发送温湿度数据函数    Call the temperature and humidity display function
  delay(1000);
}
/*
*蓝牙串口发送温湿度数据函数 sendSensorData()
*以字节方式发送
*/
void sendSensorData() {
    int chk = DHT.read11(DHT11_PIN);
    SensorData data = { DHT.temperature, DHT.humidity };    //采样温度和湿度数据   Sample temperature and humidity data
    String dataString = String(data.temperature, 1) + "," + String(data.humidity, 1);   //保留一位小数点将温湿度以逗号隔开方式拼接成字符串  Convert temperature and humidity to string
    Serial.println(dataString);       //发送温湿度数据     Send the data string
}

