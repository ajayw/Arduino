#include <IRremote.h>     //添加红外遥控库    Add infrared remote control library

#define Level0  0XFF9867  //按钮 0    Button 0
#define Level1  0XFFA25D  //按钮 1    Button 1
#define Level2  0XFF629D  //按钮 2    Button 2
#define Level3  0XFFE21D  //按钮 3    Button 3
#define Level4  0XFF22DD  //按钮 4    Button 4
#define Level5  0XFF02FD  //按钮 5    Button 5
#define Level6  0XFFC23D  //按钮 6    Button 6
#define Level7  0XFFE01F  //按钮 7    Button 7
#define Level8  0XFFA857  //按钮 8    Button 8
#define Level9  0XFF906F  //按钮 9    Button 9
#define SpeedUp   0XFF18E7//按钮 上   Button up
#define SlowDown  0XFF4AB5//按钮 下   Button down
#define OFF_ON    0XFF38C7//按钮启动    Button  OK

#define receiver 2  //定义红外接收器引脚2   Define pin 2 of the infrared receiver
#define FanPinA 6   //定义风扇电机引脚1A pin6    Define the fan motor pin 1A pin6
bool switchflag = false;//按钮开关初始设置值为关    The initial setting value of the button switch is off
int FanSpeed = 0;       //风扇速度值初始设置为0   The fan speed value is initially set to 0
IRrecv irrecv(receiver);//创建` irrecv `实例     create instance of 'irrecv'
decode_results results; //创建` decode_results `实例     create instance of 'decode_results'

void setup()
{
    pinMode(receiver,INPUT);
    irrecv.enableIRIn();      //启动红外接收器     Start the receiver
    pinMode(FanPinA, OUTPUT); //设置连接风扇电机引脚6为输出   Set pin 6 of the connected fan motor as the output
    analogWrite(FanPinA,0);   //初始设置风扇电机引脚6写入pwm为0,让电机停止
    Serial.begin(9600);       //设置波特率9600    Set the baud rate to 9600
}

void loop()
{
    if (irrecv.decode(&results))            //是否接收到红外信号   have we received an IR signal?
    {
        Serial.print("value = ");
        Serial.println(results.value, HEX); //将接收到红外编码值打印输出到串口监视器 Print the received infrared coded value to the serial port monitor
        if(results.value==OFF_ON){
          switchflag=!switchflag;           //开关按钮取反，按一下开，再按一下就关    The switch button is reversed, press the button to open, and then press the button to close
        }
        if(switchflag==true){               //在开关打开状态下    When the switch is on
            switch (results.value)          //按下不同档位按钮控制风扇不同的速度    Press the button of different gears to control the different speed of the fan
            {
            case  Level0:FanSpeed = 0;break;    //按钮0~9控制速度0~255    button 0 to 9 controls the speed 0 to 255
            case  Level1:FanSpeed = 120;break;
            case  Level2:FanSpeed = 135;break;
            case  Level3:FanSpeed = 150;break;
            case  Level4:FanSpeed = 165;break;
            case  Level5:FanSpeed = 180;break;
            case  Level6:FanSpeed = 195;break;
            case  Level7:FanSpeed = 210;break;
            case  Level8:FanSpeed = 235;break;
            case  Level9:FanSpeed = 255;break;
            case  SpeedUp:                      //按钮“上”加速、按钮“下”减速    Button "up" to speed up, button "down" to slow down
                FanSpeed += 25;
                if(FanSpeed > 255 ){
                    FanSpeed = 255;
                }
                break;
            case  SlowDown:
                FanSpeed -= 25;
                if(FanSpeed < 0 ){
                    FanSpeed = 0;
                }
                break;
            default:break;
            }
        }else{
            FanSpeed=0;     //关闭风扇 Turn off the fan
        }
        // Serial.print("speed = ");
        // Serial.println(FanSpeed);    //打印速度值到串口监视器    Print speed value to serial port monitor
        analogWrite(FanPinA,FanSpeed);  //给风扇电机引脚A写入pwm数值FanSpeed
        irrecv.resume();                //接收下一个红外信号   receive the next value
    }
}
