#include <Adafruit_NeoPixel.h>  //添加ws2812库    Add the ws2812 library
#define NUMPIXELS 4             // 定义ws2812 灯数  Number of 2812 lamps
#define  RGB_PIN  5             // 定义ws2812引脚5  ws2812 pin definition 5 

int count_G;                    //亮度变量   Brightness variable
Adafruit_NeoPixel pixels(NUMPIXELS, RGB_PIN, NEO_GRB + NEO_KHZ800);//实例化灯对象   Creating light objects

void setup ()         //设置函数    Function initialization
{
	pixels.begin();     //初始化库函数    Initialize 2812 library functions
	pixels.show();
	Serial.begin(9600);
}

void loop()           //主函数    Principal function
{
  /*!
  @brief   Set a pixel's color using separate red, green and blue
           components. If using RGBW pixels, white will be set to 0.
  @param   n  Pixel index, starting from 0.
  @param   r  Red brightness, 0 = minimum (off), 255 = maximum.
  @param   g  Green brightness, 0 = minimum (off), 255 = maximum.
  @param   b  Blue brightness, 0 = minimum (off), 255 = maximum.
  pixels.setPixelColor(n, pixels.Color(r,g,b));
  */
  for(count_G=200;count_G>0;count_G--){   //灯亮度逐渐变暗    The light dimmed
    pixels.setPixelColor(0, pixels.Color(0, count_G, 0));   //设置0~3号RGB灯颜色及亮度   Set RGB light color and brightness
    pixels.setPixelColor(1, pixels.Color(0, count_G, 0));
    pixels.setPixelColor(2, pixels.Color(0, count_G, 0));
    pixels.setPixelColor(3, pixels.Color(0, count_G, 0));
    pixels.show();
    delay(10);                            //延时10ms
  }
  for(count_G=0;count_G<200;count_G++){   //灯亮度逐渐变亮    The light is getting brighter
    pixels.setPixelColor(0, pixels.Color(0, count_G, 0));
    pixels.setPixelColor(1, pixels.Color(0, count_G, 0));
    pixels.setPixelColor(2, pixels.Color(0, count_G, 0));
    pixels.setPixelColor(3, pixels.Color(0, count_G, 0));
    pixels.show();
    delay(10);
  }
}
































