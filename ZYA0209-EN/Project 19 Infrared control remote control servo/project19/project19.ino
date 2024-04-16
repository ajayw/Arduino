#include <IRremote.h>     //添加红外遥控库    Add infrared remote control library
#include <Servo.h>        //添加舵机库    Add servo library
#define receiver 2        //定义红外接收器引脚2   Define pin 2 of the infrared receiver
#define doorOpen 0XFFE01F //定义按钮 7 的红外接收编码值   Define the infrared received coded value of button 7
#define doorClose 0XFFA857//定义按钮 8 的红外接收编码值   Define the infrared receive coded value of button 8

IRrecv irrecv(receiver);  //创建` irrecv `实例     create instance of 'irrecv'
decode_results results;   //创建` decode_results `实例     create instance of 'decode_results'
Servo myservo;            //实例化舵机为myservo create instance of 'myservo'

void setup()
{
    Serial.begin(9600);   //设置波特率9600    Set the baud rate to 9600
    irrecv.enableIRIn();  //启动红外接收器    Start the receiver
    myservo.attach(15);   //设置舵机引脚为15(A1)   attaches the servo on pin 15 to the servo object
    myservo.write(10);    //初始化设置舵机角度为10   The servo Angle is initially set to 10
}
void loop()
{
    if (irrecv.decode(&results)){
        Serial.print("value = ");
        Serial.println(results.value, HEX);   //将接收到红外编码值打印输出到串口监视器  Print the received infrared coded value to the serial port monitor
        if (results.value == doorOpen){       //根据您的IR遥控按钮号码更改  change according to your IR remote button number
            myservo.write(90);                //改变舵机的转动角度  Change the degree of rotation of the steering gear
            delay(15);
        }
        if (results.value == doorClose){      //根据您的IR遥控按钮号码更改  change according to your IR remote button number
            myservo.write(10);
            delay(15);
        }
        irrecv.resume();                      //接收下一个红外信号   receive the next value
    }
}
