#include <IRremote.h> //添加红外接收器库

#define ON  0XFFE21D  //按钮3对应红外编码保存到“ON”   Remote control button 1 value
#define OFF 0XFF22DD  //按钮4对应红外编码保存到“OFF”  Remote control button 2 value

#define RelayPin 16   //定义继电器信号引脚16(A2)  Relay control signal
#define FanPinA 7     //定义风扇电机引脚7   Motor Signal A as pin7
#define receiver 2    //定义红外接收器引脚2   Define pin 2 of the infrared receiver

IRrecv irrecv(receiver);    //创建` irrecv `实例     create instance of 'irrecv'

decode_results results;     //创建` decode_results `实例     create instance of 'decode_results'

void setup()
{
    Serial.begin(9600);
    irrecv.enableIRIn();            //启动红外接收器     Start the receiver

    pinMode(FanPinA, OUTPUT);       //设置电机引脚为输出    Set the motor pin to the output
    pinMode(RelayPin, OUTPUT);      //设置继电器信号引脚为输出    Set the relay signal pin as the output
    digitalWrite(RelayPin, LOW);    //初始设置继电器低电平、电机引脚为高电平    The relay pins are initially set to low level and motor pins set to high
    digitalWrite(FanPinA, LOW);    //风扇电机信号引脚7写入低电平   Fan motor pin 7 write low level
}

void loop()
{
  if (irrecv.decode(&results)){
      Serial.print("value = ");
      Serial.println(results.value, HEX);
      if (results.value == ON )       //打开继电器    Turn on relay
      {
          digitalWrite(RelayPin, HIGH);
          digitalWrite(FanPinA, HIGH);
      }
      else if (results.value == OFF ) //关闭继电器    Shut off relay
      {
          digitalWrite(RelayPin, LOW);
          digitalWrite(FanPinA, LOW);
      }
      irrecv.resume(); //接收下一个红外编码值    Receive the next value
  }
}
