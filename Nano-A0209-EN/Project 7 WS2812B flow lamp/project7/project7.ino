#include <Adafruit_NeoPixel.h>  //添加ws2812库          Add the ws2812b library
#define NUMPIXELS 4             // 定义ws2812 灯数      Number of ws2812b lamps
#define RGB_PIN  5              // 定义ws2812引脚5      ws2812b pin definition 5

Adafruit_NeoPixel pixels(NUMPIXELS, RGB_PIN, NEO_GRB + NEO_KHZ800);//实例化灯对象   Creating light objects

//Variable definition
int rotate=0;   //亮同种颜色灯的循环次数    Number of cycles to turn on a light of the same color

//Initialization function
void setup ()
{
	pixels.begin(); //初始化库函数    Initialize 2812 library functions
	pixels.show();
	Serial.begin(9600);
	pixels.clear(); //初始清除
}

void loop()       //主函数    Principal function
{
	if(rotate<=4)
		{
			for (int j=0;j<=3;j++)    //亮红灯    Flashing a red light
			{		
				pixels.setPixelColor(j, pixels.Color(150, 0, 0));
				pixels.show();
				delay(100);
				pixels.setPixelColor(j, pixels.Color(0, 0, 0));
				pixels.show();
				delay(10);
			}
			rotate++;                 //红色灯走完4盏灯后加1    Add 1 after the red light has gone 4 lights
		}
		if((rotate>=5)&&(rotate<10))//亮绿灯    Flashing a green light
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
		if(rotate>=10)              //亮蓝灯    Flashing a blue light
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
		if(rotate>=15)rotate=0;   //重置循环参数    Resetting loop parameters
}
