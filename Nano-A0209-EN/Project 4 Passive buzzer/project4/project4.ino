
#define  buzzer 3         //定义蜂鸣器输出3引脚 Define buzzer output pin
void setup ()
{
	pinMode(buzzer,OUTPUT); //设置引脚为输出 Set pin to output
}

void loop()
{
  digitalWrite(buzzer,HIGH);//3引脚输出高电平 3 pin output high level
  delay(500);
  digitalWrite(buzzer,LOW); //3引脚输出低电平 3 pin output low level
  delay(500);

/*
*tone(buzzer,523);
*523为频率，不同的数值代表不一样的频率，修改该数值可以得到不同声调的声音
*523 is the frequency, different values represent different frequencies, modify the value to get different tones of the voice
*/
  // tone(buzzer,523);       //3引脚输出pwm 频率为523  The output pwm frequency of the 3-pin is 523
  // delay(1000);
  // noTone(buzzer);         //关闭pwm输出
  // delay(1000);

}

