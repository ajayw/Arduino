#include <Adafruit_NeoPixel.h>  //添加ws2812库          Add the ws2812b library
#define NUMPIXELS 4             // 定义ws2812 灯数      Number of ws2812b lamps
#define RGB_PIN  5              // 定义ws2812引脚5      ws2812b pin definition 5 
#define KEY_1   20              // 定义触控模块引脚A6    Key pin definition A6

Adafruit_NeoPixel pixels(NUMPIXELS, RGB_PIN, NEO_GRB + NEO_KHZ800);//实例化灯对象   Creating light objects

//Variable definition
int i;          //触控值变量    Touch value variable
int count_R=0;  //R/G/B变量初始值   Initial values for R/G/B variables
int count_G=0;
int count_B=0;
int count_key=0;//触控次数变量    Touch count variable
int Press=0;    //触控标志变量    Touch flag variables

//Initialization function
void setup ()
{
	pinMode(KEY_1,INPUT);
	pixels.begin(); //初始化库函数    Initialize 2812 library functions
	pixels.show();
	Serial.begin(9600);
	pixels.clear(); //初始清除
}

void loop()       //主函数    Principal function
{
	KEY();          //按键函数    Key function
	RGB();          //亮灯函数    RGB function
}
https://www.dropbox.com/scl/fo/qaoa6ohxiarn4l38boxf9/h?rlkey=14xz22m4qu4186q1y8crf1v2v&dl=0

void RGB()
{
	switch(count_key)     //第几次触摸    Number of key presses
	{
		case 1 :            //逐渐亮红灯    The red light is on
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
		case 2 :            //逐渐亮绿灯    The green light is on
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
		case 3 :            //逐渐亮蓝灯    The blue light is on
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
    case 4 :            //亮白灯    The white light is on
      pixels.setPixelColor(0,pixels.Color(100, 100, 100));
      pixels.setPixelColor(1,pixels.Color(100, 100, 100));
      pixels.setPixelColor(2,pixels.Color(100, 100, 100));
      pixels.setPixelColor(3,pixels.Color(100, 100, 100));
      pixels.show();
      break;
    case 5 :            //灭灯    The light is off
      pixels.setPixelColor(0, pixels.Color(0, 0, 0));
      pixels.setPixelColor(1, pixels.Color(0, 0, 0));
      pixels.setPixelColor(2, pixels.Color(0, 0, 0));
      pixels.setPixelColor(3, pixels.Color(0, 0, 0));
      pixels.show();
      break;
		default : break;
	}
}

void KEY()                  //触控消抖函数  Key function
{
  i=analogRead(KEY_1);      //A6引脚使用读取模拟方法获取触摸模块数值    Pin A6 uses the read simulation method to obtain the touch module value
	if((i<300)&&(Press==0)){  
    	Press=1;                //按下标志==1   Press the sign
	}
  i=analogRead(KEY_1);
  if((i>1000) &&(Press==1)) //松开触控按钮且按下标志==1   Let go detection
  {
    // Serial.print("i:");
    // Serial.println(i);
    // Serial.print("count_key:");
    // Serial.println(count_key);
    Press=0;
    count_key++;
    if(count_key>5)count_key=1;//超过5次回到初始值1   Return to the initial value of 1 more than 5 times
  }
}

