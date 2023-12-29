/*

产品：ZYA0210
按钮和光敏互联智能控制灯带

*/
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal_I2C.h>
#define PIN           A0  // 修改为A0作为数据引脚 It is modified to A0 as the data pin
#define NUMPIXELS     30  // 修改LED的数量为12 The number of leds is modified to 12
#define buttonPin     2   // 按钮引脚 Button pin
#define Light         A3  // 光敏电阻检测引脚 Photoresistor detection pin
bool Light_mode = true;
bool FristBtn = true;
bool btn_switch =true;
bool buttonState=1;     //定义按钮状态  Define key status bits
bool lastButtonState=1; //上一次按键状态  Last key press status
int sensorValue1;
int color_mode=1;
unsigned long btn_time=0;
unsigned long btn_latetime=0;
unsigned long delayTime;

LiquidCrystal_I2C lcd(0x27, 16, 2); // 设置I2C地址为0x27，LCD有16列和2行 The I2C address is set to 0x27 and the LCD has 16 columns and 2 rows

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
void setup ()           //设置函数  Function initialization
{
  strip.begin();        // 初始化NeoPixel库 Initialize the NeoPixel library
  strip.show();         // 初始化时关闭所有LED按键被按下 Initialize when turn off all LED keys are pressed
  Serial.begin(9600);   // 初始化串口 Initialize the serial port
  lcd.init();           // 初始化LCD Initialize LCD
  lcd.backlight(); 
  lcd.clear();                      // 清除LCD屏幕内容 Clear the LCD screen contents
  lcd.setCursor(3, 0);              // 设置光标位置为第一行第一列 Set the cursor position to the first row and column
  lcd.print("Christmas");           // 在LCD上打印 "Christmas" Print "Christmas" on the LCD
  // 在此处可以设置亮度
  strip.setBrightness(100);
  pinMode(buttonPin, INPUT_PULLUP); //配置按键为上拉输入 The configuration button is a pull-up input
  pinMode(Light, INPUT);            //设置为输入引脚 Set to the input pin
}

void loop()   //Principal function
{
  sensorValue1 = analogRead(Light);
  Serial.println(sensorValue1);
  if(sensorValue1 >= 800 && Light_mode){  //检测环境亮度 如过检测数值大于800 Detect ambient brightness such as over-detection value greater than 800
    color_mode = 5;
    Light_mode = false;
    btn_switch =true;
  }else if(sensorValue1<=500 && !Light_mode){
    color_mode = 0;                        //设定为模式1，即白天 Set to mode 1, which is daytime
    Light_mode = true;
    btn_switch = false;
  }

  buttonState = digitalRead(buttonPin);     //读取按键状态  Reading buttonPin state (A7引脚只支持读取模拟值不支持读数字值)
  if (buttonState != lastButtonState) {     //检测按键状态是否发生改变  Detect if the buttonPin state has changed
    delay(20);                              //等待一段时间以避免按键抖动  Wait a while to avoid buttonPin jitter
    buttonState = digitalRead(buttonPin);   //再次读取按键状态进行确认  Read the key state again for confirmation
    if (buttonState != lastButtonState) {   //检测确认后的按键状态  Check the status of the key after confirmation
      if (!buttonState && FristBtn) {       //按键被按下  The key is pressed
        btn_time = millis();
        FristBtn = false;
      }else if(buttonState && !FristBtn){
        FristBtn = true;
        btn_latetime = millis();
        delayTime = btn_latetime - btn_time;
        if(delayTime<1000){
          if(btn_switch){
            color_mode++;                    //反转LED灯状态 Reverse the LED state
            if(color_mode>5){
              color_mode = 1;
            }
          }
        }else if(delayTime>1000){
          if(btn_switch){
            btn_switch = false;
            color_mode = 0;
          }else{
            btn_switch = true;
            color_mode = 1;
          }
        }
      }
    }
  }
  lastButtonState = buttonState;      //更新上一次按键状态

//判断该显示的颜色
  if(color_mode == 0){
    // 将所有RGB关闭
    colorWipe(strip.Color(0, 0, 0), 1); // 1mm延时
  }else if(color_mode == 1){
    // 将所有RGB的颜色设置为红色  Set the color of all RGBs to red
    colorWipe(strip.Color(255, 0, 0), 1);
  }else if(color_mode == 2){
    // 将所有RGB的颜色设置为绿色  Set the color of all RGBs to green
    colorWipe(strip.Color(0, 255, 0), 1);
  }else if( color_mode == 3){
    // 将所有RGB的颜色设置为蓝色  Set the color of all RGBs to blue
    colorWipe(strip.Color(0, 0, 255), 1);
  }else if(color_mode == 4){
    // 将所有RGB的颜色设置为白色  Set the color of all RGBs to white
    colorWipe(strip.Color(100, 100, 100), 1);
  }else if(color_mode == 5){
    // 将所有RGB的颜色设置为黄色  Set the color of all RGBs to yellow
    colorWipe(strip.Color(180, 75, 0), 1);
  }
}
// 使所有RGB为指定颜色  Set all RGB to the specified color
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
    strip.show();
    delay(wait);
  }
}