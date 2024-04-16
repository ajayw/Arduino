#define LED_Pin 8             //定义LED引脚8    Define the LED pin 8
bool LED_Status = LOW;        //定义LED初始状态低电平   Define the LED initial state low level
char ser_val;                 //定义接收蓝牙信息的字符变量    Defines a character variable that receives Bluetooth information

void setup() {
  Serial.begin(9600);
  pinMode(LED_Pin, OUTPUT);   //设置LED引脚为输出   Set the LED pin as the output
  digitalWrite(LED_Pin, LOW); //设置LED初始状为灭灯   Set the LED to be off initially
}

void loop() {
  if(Serial.available() > 0)  //判断串口是否接收到蓝牙信息    Determine whether the serial port has received Bluetooth information
  {
    ser_val = Serial.read();  //将接收到的蓝牙信息保存到变量ser_val   Save the received Bluetooth information to the variable ser_val
    if(ser_val == 'K')        //如果接收到的信息是字符“K”，证明按下正确的按钮   If the received message is the character "K", prove that the correct button was pressed
    {
      LED_Status = digitalRead(LED_Pin);  //获取LED的状态  Gets the LED status
      // Serial.print("LED_Status=");
      // Serial.println(LED_Status);
      if(!LED_Status){                    //如果LED在灭灯状态就打开，如果是在打开状态就灭灯   If the LED is off, turn it on. If it's on, turn it off
        digitalWrite(LED_Pin, HIGH);
      }else{
        digitalWrite(LED_Pin, LOW);
      }
    }
  }
}