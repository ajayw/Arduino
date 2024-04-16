#include "IRremote.h"
#include <Adafruit_NeoPixel.h>
#define NUMPIXELS 4   // 定义ws2812 灯数      Number of ws2812b lamps
#define RGB_PIN  5   // 定义ws2812引脚5      ws2812b pin definition 5
#define receiver 2   // 定义红外接收器引脚2   Define pin 2 of the infrared receiver

Adafruit_NeoPixel pixels(NUMPIXELS, RGB_PIN, NEO_GRB + NEO_KHZ800);//实例化灯对象   Creating light objects

IRrecv irrecv(receiver);        //创建` irrecv `实例     create instance of 'irrecv'
decode_results results;         //创建` decode_results `实例     create instance of 'decode_results'

//Variable definition
int count_R=0;                  //R/G/B变量初始值   Initial values for R/G/B variables
int count_G=0;
int count_B=0;
int count_key=0;                //红外按键赋值变量    Infrared key assignment variable
int rotate=0;                   //流水灯序号        Serial number of running lamp

//Initialization function
void setup ()
{
	pinMode(receiver,INPUT);      //设置红外接收器引脚为输入    Set the infrared receiver pin as input
	pixels.begin();               //初始化ws2812库函数     Initialize ws2812 library functions
	pixels.show();
	irrecv.enableIRIn();          //启动红外接收器     Start the receiver
	Serial.begin(9600);           //设置波特率9600    Set the baud rate to 9600
}

void loop()
{
  if (irrecv.decode(&results))  //是否接收到红外信号   have we received an IR signal?
  {
    translateIR(); 
    Serial.println(results.value); //将接受到红外编码值打印输出到串口监视器 Print the received infrared coded value to the serial port monitor
    irrecv.resume();            //接收下一个红外信号   receive the next value
  } 
	pixels.clear();               //照明功能   Lighting function
	RGB();                        //RGB函数    RGB function
}

void RGB()
{
	switch(count_key)             //判断红外按键赋值变量    Determine the infrared key assignment variable
	{
		case 1 :          //亮红灯    The red light is on
			delay(10);
			count_R++;
			if(count_R>=255)count_R=255;
			count_G=0;
			count_B=0;
			pixels.setPixelColor(0, pixels.Color(count_R, 0, 0));
			pixels.setPixelColor(1, pixels.Color(count_R, 0, 0));
			pixels.setPixelColor(2, pixels.Color(count_R, 0, 0));
			pixels.setPixelColor(3, pixels.Color(count_R, 0, 0));
			pixels.show();
			break;
		case 2 :          //亮绿灯    The green light is on
			delay(10);
			count_G++;
			if(count_G>=255)count_G=255;
			count_R=0;
			count_B=0;
			pixels.setPixelColor(0, pixels.Color(0, count_G, 0));
			pixels.setPixelColor(1, pixels.Color(0, count_G, 0));
			pixels.setPixelColor(2, pixels.Color(0, count_G, 0));
			pixels.setPixelColor(3, pixels.Color(0, count_G, 0));
			pixels.show();
			break;
		case 3 :          //亮蓝灯    The blue light is on
			delay(10);
			count_B++;
			if(count_B>=255)count_B=255;
			count_R=0;
			count_G=0;
			pixels.setPixelColor(0, pixels.Color(0, 0, count_B));
			pixels.setPixelColor(1, pixels.Color(0, 0, count_B));
			pixels.setPixelColor(2, pixels.Color(0, 0, count_B));
			pixels.setPixelColor(3, pixels.Color(0, 0, count_B));
			pixels.show();
			break;
		case 4 :          //亮白灯    The white light is on
			//delay(100);
			pixels.setPixelColor(0, pixels.Color(255, 255, 255));
			pixels.setPixelColor(1, pixels.Color(255, 255, 255));
			pixels.setPixelColor(2, pixels.Color(255, 255, 255));
			pixels.setPixelColor(3, pixels.Color(255, 255, 255));
			pixels.show();
			break;
		case 5 :          //关闭所有灯    Turn off the light
			pixels.setPixelColor(0, pixels.Color(0, 0, 0));
			pixels.setPixelColor(1, pixels.Color(0, 0, 0));
			pixels.setPixelColor(2, pixels.Color(0, 0, 0));
			pixels.setPixelColor(3, pixels.Color(0, 0, 0));
			pixels.show();
			break;
		case 6 :          //红、绿、蓝交替闪烁    Red, green and blue flicker alternately	
		if(rotate<=4)
		{
			for (int j=0;j<=3;j++)
			{		
				pixels.setPixelColor(j, pixels.Color(150, 0, 0));
				pixels.show();
				delay(100);
				pixels.setPixelColor(j, pixels.Color(0, 0, 0));
				pixels.show();
				delay(10);
			}
			rotate++;
		}
		if((rotate>=5)&&(rotate<10))
		{
			for (int j=0;j<=3;j++)
			{		
				pixels.setPixelColor(j, pixels.Color(0, 150, 0));
				pixels.show();
				delay(100);
				pixels.setPixelColor(j, pixels.Color(0, 0, 0));
				pixels.show();
				delay(10);
			}
			rotate++;
		}
		if(rotate>=10)
		{
			for (int j=0;j<=3;j++)
			{		
				pixels.setPixelColor(j, pixels.Color(0, 0, 150));
				pixels.show();
				delay(100);
				pixels.setPixelColor(j, pixels.Color(0, 0, 0));
				pixels.show();
				delay(10);
			}
			rotate++;
		}
		if(rotate>=15)rotate=0;
		break;
		default : break;
	}
}

void translateIR() //根据收到的IR代码给count_key赋不同的值    takes action based on IR code received
{
  switch(results.value)
  {
    case 0XFFA25D: count_key=1; break;    //接收到按键1    Key 1 received
    case 0XFF629D: count_key=2; break;    //接收到按键2    Key 2 received
    case 0XFFE21D: count_key=3; break;    //接收到按键3    Key 3 received
    case 0XFF22DD: count_key=4; break;    //接收到按键4    Key 4 received
    case 0XFF02FD: count_key=5; break;    //接收到按键5    Key 5 received
    case 0XFFC23D: count_key=6; break;    //接收到按键6    Key 6 received
    default: break;
  }
}

