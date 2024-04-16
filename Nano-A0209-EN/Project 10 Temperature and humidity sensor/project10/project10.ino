#include <Wire.h> 
#include <dht.h>                //添加温湿度库    Add temperature and humidity library
#define DHT11_PIN 4 //定义温湿度传感器引脚4   Define temperature and humidity sensor pin4
dht DHT;            //实例化温湿度对象为DHT   Instantiate the temperature and humidity object as a DHT

void setup()
{
  Serial.begin(9600);
  pinMode(DHT11_PIN,INPUT);
}
void loop()
{
    int chk = DHT.read11(DHT11_PIN);  //读取温湿度    Reading temperature and humidity 
    Serial.print("temp:");
    Serial.print(DHT.temperature,1);  //打印温度值    print the temprature value
    Serial.print("--humi:");
    Serial.println(DHT.humidity,1);   //打印湿度值    print the humidity value
    delay(200);
}
