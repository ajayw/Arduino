#define  LED  8   // 定义8引脚为LED  Define output pin
#define  KEY  A7  // 定义A7引脚为KEY  Define input pin

bool buttonState=1;     //定义按钮状态  Define key status bits
bool lastButtonState=1; //上一次按键状态  Last key press status
bool LED_BIT=0;         //定义led状态  Define led status bits

void setup ()           //设置函数  Function initialization
{
	pinMode(LED,OUTPUT);  //定义LED引脚为输出  Define output
	pinMode(KEY,INPUT);   //定义KEY引脚为输入  Define input
}

void loop()   //Principal function
{
  buttonState = analogRead(KEY);          //读取按键状态  Reading key state (A7引脚只支持读取模拟值不支持读数字值)
  if (buttonState != lastButtonState) {   //检测按键状态是否发生改变  Detect if the key state has changed
    delay(50);                            //等待一段时间以避免按键抖动  Wait a while to avoid key jitter
    buttonState = analogRead(KEY);        //再次读取按键状态进行确认  Read the key state again for confirmation
    if (buttonState != lastButtonState) { //检测确认后的按键状态  Check the status of the key after confirmation
      if (buttonState == LOW) {           //按键被按下  The key is pressed
        LED_BIT = !LED_BIT;               //反转LED灯状态 Reverse the LED state
      }
    }
  }
  lastButtonState = buttonState;      //更新上一次按键状态
	if(LED_BIT) digitalWrite(LED,HIGH); //判断LED_BIT==1时,输出高电平灯亮 The output of single chip microcomputer is high
	else digitalWrite(LED,LOW);         //否则输出低电平灯灭  Low output of single chip microcomputer
}
