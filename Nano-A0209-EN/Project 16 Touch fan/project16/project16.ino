#define RelayPin  A2      //定义继电器引脚A2  Relay A2
#define FanPinA  7        //定义风扇引脚1A  L9110 1A
#define touch    A6       //定义按钮引脚A6  Define input pin

bool state = 0;            //继电器状态标志变量    Relay state flag variable
int Press = 1;            //触控按钮标志变量    Touch button flag variables
int value = 1;            //触控按钮值，触控感应>1000为触控状态，<300为松开状态   Touch button value, touch induction &gt; 1000 is touch state, &lt; 300 is the release state

void setup() 
{
  Serial.begin(9600);
  pinMode(RelayPin, OUTPUT);  //设置继电器引脚为输出    Set the relay pin as the output
  pinMode(FanPinA, OUTPUT);   //设置风扇引脚为输出    Set the fan pin as the output
  pinMode(touch , INPUT);     //设置按钮引脚为输入    Set the button pin as input
  analogWrite(FanPinA,LOW);  //风扇信号引脚设置高电平    Set the fan pin as the high level
}

void loop()
{
  value = analogRead(touch);        //获取触控按钮值    Gets the touch button value
  // Serial.println(value);
  Serial.println(state);
  //Serial.println(Press);
  if(value < 300){                  //判断触控按钮没有在按下触控状态    Determine that the touch button is not in touch pressed state
    Press = 1;
  }
  if((value > 1000)&&(Press == 1))  //触控按钮值>1000且Press == 1，触控按钮被触控一次   Touch button values &gt; 1000 and Press == 1, the touch button has been touched once
  {
    Press = 0;
    state = !state;
  }
  if(state == 1)digitalWrite(RelayPin,HIGH);analogWrite(FanPinA,HIGH);  //继电器闭合风扇开启    Turn on the fan
  else digitalWrite(RelayPin, LOW);analogWrite(FanPinA,LOW);            //继电器断开风扇停止    Turn off the fan
}
