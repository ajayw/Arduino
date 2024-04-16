#define FanPinA 6       //定义风扇引脚5 Define fan pin 6
int speed = 145;        //设置初始速度 Set the initial speed
bool fan_switch = false;//设置风扇开关状态 Set the fan switch status

void setup() {
  Serial.begin(9600);
  pinMode(FanPinA, OUTPUT);
  analogWrite(FanPinA,LOW); //初始设置引脚6为低电平 Set pin 6 to low
}

void loop() {
  if(Serial.available() > 0) 
  {    
    char ser_val = Serial.read();//读取蓝牙数据 Read Bluetooth data
    if(ser_val == 'G')    //如果接收到的数据是G If the received data is G
    {
      speed = 145;        //每次开关都重置速度，该行可删除并保留上一速度值
      fan_switch = !fan_switch;//状态转换 State transitions
    }
    if(fan_switch)        //如果开关状态为"真" If the switch state is "ture"
    {
      if(ser_val == 'U')  //如果接收到的数据是U If the received data is U
      {
        speed += 25;      //速度增加25 Speed increased by 25
        if(speed >= 245)  //如果速度大于等于245就等于245 If the speed is greater than or equal to 245, it is equal to 245
        {
          speed = 245;
        }
      }
      if(ser_val == 'D')  //如果接收到的数据是D If the received data is D
      {
        speed -= 25;      //减少速度25 Reduce the speed by 25
        if(speed <= 95)   //如果速度小于等于95就等于95 If the speed is less than or equal to 95, it is equal to 95
        {
          speed = 95;
        }
      }
      analogWrite(FanPinA, speed);
    }else {analogWrite(FanPinA, LOW);}
    Serial.println(speed);
  }
}
