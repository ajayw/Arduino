#include <Servo.h> 
bool doorStatus = false;      //门状态初始设置    Initial setting of the gate state
Servo doorServo;              //用于门锁机构的伺服    Used for servo of door lock mechanism

void setup() {
  Serial.begin(9600);         //设置波特率 Set the baud rate
  doorServo.attach(A1);       //设置舵机引脚A1    Set servo pin A1
  doorServo.write(0);         //将舵机移动到锁定位置 Move servo into locked position
}

void loop() {
  if(Serial.available() > 0) 
  {    
    char ser_val = Serial.read(); //读取蓝牙数据 Read Bluetooth data
    if(ser_val == 'H'){           //如果接收到的数据是H If the received data is H
      doorStatus = !doorStatus;   //状态反转 Inversion of status
      if(doorStatus){               //如果door状态为"真" If the door status is "true" 
        for(int i = 10 ; i < 90 ; i++ ){//循环执行90次增加度数来开门 The loop is performed 90 times increasing the degree to open the door
          delay(10);
          doorServo.write(i);
        }
      }else{                            //如果door状态为"假" If the door status is “false” 
        for(int i = 90 ; i > 10 ; i--){ //循环执行90次减少度数来关门 The loop is performed 90 times increasing the degree to close the door
          delay(10);
          doorServo.write(i);
        } 
      }
    }
  }
}
