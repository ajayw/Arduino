
int buzzer = 3;         //定义蜂鸣器引脚编号为Type-C Nano引脚3    Define the buzzer pin number to the pin3 of Type-C Nano
int smokeA0 = A0;       //定义MQ-2气体传感器引脚连接到Type-C纳米模拟端口的A0引脚     Define the MQ-2 gas sensor pin to connect to the A0 pin of the Type-C Nano analog port
int sensorThres = 100;  //气体阈值     Your threshold value

void setup() {
  pinMode(buzzer, OUTPUT);  //定义蜂鸣器引脚为输出    Define the buzzer pin as the output
  pinMode(smokeA0, INPUT);  //定义气体传感器引脚为输入    Define the gas sensor pin as the input
  Serial.begin(9600);
}

void loop() {
  int analogSensor = analogRead(smokeA0); //MQ-2传感器检查值被分配给ALOG传感器变量    The MQ-2 sensor check values are assigned to the ALOG Sensor variable
  Serial.print("Pin A0: ");
  Serial.println(analogSensor);           //串口打印传感器数值    The serial port prints the sensor value
  if (analogSensor > sensorThres)         //检查是否达到阈值    Checks if it has reached the threshold value
  {
    tone(buzzer, 1000, 200);              //蜂鸣器响    The buzzer sounded
  }else{
    noTone(buzzer);                       //蜂鸣器停    The buzzer stops
  }
  delay(100);
}
