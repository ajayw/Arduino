
#include <Adafruit_NeoPixel.h>
#define NUMPIXELS 4   // 定义ws2812 灯数      Number of ws2812b lamps
#define RGB_PIN  5    // 定义ws2812引脚5      ws2812b pin definition 5

Adafruit_NeoPixel pixels(NUMPIXELS, RGB_PIN, NEO_GRB + NEO_KHZ800);//实例化灯对象   Creating light objects

//Variable definition
int count_R=0;                  //R/G/B变量初始值   Initial values for R/G/B variables
int count_G=0;
int count_B=0;
int ser_val=0;                  //蓝牙按键信息赋值变量    Bluetooth key information assignment variable
int rotate=0;                   //流水灯序号        Serial number of running lamp

//Initialization function
void setup ()
{
	pixels.begin();               //初始化ws2812库函数     Initialize ws2812 library functions
	pixels.show();
	Serial.begin(9600);           //设置波特率9600    Set the baud rate to 9600
}

void loop()
{
  if (Serial.available() > 0)   //是否接收到蓝牙信号   have we received an Bluetooth signal?
  {
    ser_val = Serial.read();    //将接收到的蓝牙信息保存到变量ser_val   Save the received Bluetooth information to the variable ser_val
    Serial.println(ser_val);    //将接受到蓝牙信息打印输出到串口监视器 Print the received infrared coded value to the serial port monitor
  } 
	// pixels.clear();               //照明功能   Lighting function
	RGB();                        //RGB函数    RGB function
}

void RGB()
{
	switch(ser_val)             //判断蓝牙按键赋值变量    Determine the Bluetooth key assignment variable
	{
		case 'A' :          //亮白灯    The white light is on
			pixels.setPixelColor(0, pixels.Color(180, 180, 180));
			pixels.setPixelColor(1, pixels.Color(180, 180, 180));
			pixels.setPixelColor(2, pixels.Color(180, 180, 180));
			pixels.setPixelColor(3, pixels.Color(180, 180, 180));
			pixels.show();
			break;
		case 'B' :          //亮红灯    The red light is on
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
		case 'C' :          //亮绿灯    The green light is on
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
		case 'D' :          //亮蓝灯    The blue light is on
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
		case 'E' :          //关闭所有灯    Turn off the light
			pixels.setPixelColor(0, pixels.Color(0, 0, 0));
			pixels.setPixelColor(1, pixels.Color(0, 0, 0));
			pixels.setPixelColor(2, pixels.Color(0, 0, 0));
			pixels.setPixelColor(3, pixels.Color(0, 0, 0));
			pixels.show();
			break;
		case 'K' :          //红、绿、蓝流动闪烁    Red, green and blue flicker alternately	
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
