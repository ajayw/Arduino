
#define  LED  8   //定义LED信号引脚 Define output pin

void setup ()     //函数初始化 Function initialization
{
	pinMode(LED,OUTPUT);  //定义8引脚作为输出 Define output
}

void loop()             //循环主函数 Principal function
{
		digitalWrite(LED,HIGH); //8引脚输出高电平 8 pin output high level
		delay(500);             //延时500毫秒     The delay is 500 milliseconds
		digitalWrite(LED,LOW);  //8引脚输出低电平 8 pin output low level
		delay(500);             //延时500毫秒     The delay is 500 milliseconds
}
