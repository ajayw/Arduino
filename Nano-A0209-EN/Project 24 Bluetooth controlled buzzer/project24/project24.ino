
#define buzzer 3              //定义蜂鸣器引脚3
bool buzzer_Status = false;   //设置蜂鸣器状态变量
char ser_val;                 //定义接收蓝牙信息的字符变量    Defines a character variable that receives Bluetooth information
void setup() {
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
}

void loop() {
  if(Serial.available() > 0){     //判断串口是否接收到蓝牙信息    Determine whether the serial port has received Bluetooth information
    char ser_val = Serial.read(); //将接收到的蓝牙信息保存到变量ser_val   Save the received Bluetooth information to the variable ser_val
    //Serial.print("ser_val=");
    //Serial.println(ser_val);
    if(ser_val == 'L'){           //如果接收到的信息是字符“L”，证明按下正确的按钮   If the received message is the character "L", prove that the correct button was pressed
      buzzer_Status = !buzzer_Status;
    }
    if(buzzer_Status)tone(buzzer, 523);  //按一次按钮蜂鸣器响，在按一次停止响    Press the button once to sound the buzzer, and press the button once to stop the buzzer
    else noTone(buzzer);
  }
}
