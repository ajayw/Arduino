#include <Servo.h>
#define rain_Pin A3             //定义雨滴传感器引脚A3（17）
int rain;                       //雨滴传感器数据采集变量    Raindrop sensor data acquisition variables
int pos = 0;                    //变量来存储伺服位置    variable to store the servo position
bool windowFlag = 0;            //窗户状态标志    Window status flag
Servo myservo;                  //实例化舵机为myservo   create instance of 'myservo'

void setup()
{
    pinMode(rain_Pin, INPUT);   //设置雨滴传感器引脚A3为输入    set the raindrop sensor pin A3 as the input
    myservo.attach(6);          //设置舵机引脚为6   attaches the servo on pin 6 to the servo object
    Serial.begin(9600);         //设置波特率9600    Set the baud rate to 9600
}

void loop()
{
    /*
    *传感器数据采集（0~1023）    Acquisition of sensor data
    *下雨时0~800，不下雨时大于800    0 to 800 when it rains and greater than 800 when it does not rain
    */
    rain = analogRead(rain_Pin);              
    Serial.print("RainData =");
    Serial.println(rain);
    
    if(rain < 800  && !windowFlag){           //下雨时，舵机驱动关闭窗户    The steering gear turns to close the window in the rain
        for ( pos = 10; pos < 130; pos++){
            myservo.write(pos);
            delay(10);
        }
        windowFlag = 1;
    }else if(rain > 800 && windowFlag){       //不下雨时，舵机驱动打开窗户    The steering gear turns without rain, open the window
        for ( pos = 130; pos >= 10; pos--){
            myservo.write(pos);
            delay(10);
        }
        windowFlag = 0;
    }
}
