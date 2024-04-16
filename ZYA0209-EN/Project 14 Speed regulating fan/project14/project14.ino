/*
Speed fan experiment, slowly accelerate, slow down.
*/

#define FanPin 6              //定义风扇电机信号引脚6   Define fan motor signal pin 6

void setup()
{
    Serial.begin(9600);
    pinMode(FanPin, OUTPUT);  //设置电机信号引脚6为输出   Configure the pin mode
}

void loop()
{
    for (int i = 30; i <= 255; i++)//速度从30增加到255   Speed increased from 30 to 255
    {
        Serial.print("speed = ");
        Serial.println(i);
        analogWrite(FanPin,i);
        delay(50);
    }
    for (int i = 255; i >= 30; i--)//速度从255降低到30   Speed reduced from 255 to 30
    {
        Serial.print("speed = ");
        Serial.println(i);
        analogWrite(FanPin,i);
        delay(50);
    }
}
