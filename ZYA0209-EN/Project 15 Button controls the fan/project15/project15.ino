#define RelayPin  A2      //定义继电器引脚A2  Relay A2
#define FanPinA  7        //定义风扇引脚1A  L9110 1A
#define  KEY     A7       //定义按钮引脚A7  Define input pin

int buttonState=1;        //定义按钮状态  Define key status bits
int lastButtonState=1;    //上一次按键状态  Last key press status
bool Relay_BIT=false;     //继电器状态    Relay status

void setup() //初始化设置   Function initialization
{
  Serial.begin(9600);
  pinMode(RelayPin, OUTPUT);  //设置继电器引脚为输出    Set the relay pin as the output
  pinMode(FanPinA, OUTPUT);   //设置风扇引脚为输出    Set the fan pin as the output
	pinMode(KEY,INPUT);         //设置按钮引脚为输入    Set the button pin as input
  analogWrite(FanPinA,LOW);  //风扇信号引脚设置低电平    Set the fan pin as the low level
}

void loop() //主函数    Principal function
{
  buttonState = analogRead(KEY);        //获取按钮状态    Getting the button status
  if(buttonState != lastButtonState)    //按钮消抖    Button chattering
  {
    delay(20);
    buttonState = analogRead(KEY);
    if(buttonState != lastButtonState)
    {
      if(buttonState == LOW)
      {
        Relay_BIT = !Relay_BIT;
      }
    }
  }
  lastButtonState = buttonState;
  if (Relay_BIT){                       //判断Relay_BIT是否为“真”   Determine whether Relay_BIT is true
    digitalWrite(RelayPin, HIGH);       //开启风扇    Turn on the fan
    analogWrite(FanPinA, HIGH);
  }else {digitalWrite(RelayPin, LOW);analogWrite(FanPinA,LOW);}    //关闭风扇    Turn off the fan
}