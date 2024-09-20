#include<SoftwareSerial.h>
void setup() {
  Serial.begin(9600);
}

void loop() {
  if(Serial.available() > 0) {      //Determine whether the received data is greater than 0
    char ser_val = Serial.read();
    Serial.print("ser_val=");
    Serial.println(ser_val);
    switch(ser_val)
      {
          case 'F':
           Serial.println("前进");
          break;
          case 'B':
           Serial.println("后退");
          break;
          case 'S':
          Serial.println("停止");
          break;
          case 'L':
          Serial.println("左转");
          break;
          case 'R':
          Serial.println("右转");
          break;
          default:
          break;
      }   
    }
  
}

