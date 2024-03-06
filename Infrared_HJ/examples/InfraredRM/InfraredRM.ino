/*************************************************************
*  红外遥控库示例 v0.1.3
*  Date: 2024.3
*  Author: Ajay Huajian
*
*  【遥控器按钮对应键码】
*  MENU B847FF00
*  TEST BB44FF00
*  Back BC43FF00
*  +    BF40FF00
*  -    E619FF00
*  <<   F807FF00
*  >>   F609FF00
*  RUN  EA15FF00
*  C    F20DFF00
*  0    E916FF00
*  1    F30CFF00
*  2    E718FF00
*  3    A15EFF00
*  4    F708FF00
*  5    E31CFF00
*  6    A55AFF00
*  7    BD42FF00
*  8    AD52FF00
*  9    B54AFF00
* 
* HW-477 V0.2红外接收器，白色20键红外遥控器
* 适用于UNO/NANO/UNO R4等Arduino主控板
* 串口波特率设置为115200
* 执行代码后打开串口监视器查看按钮键码，根据键码可以实现条件判断控制LED等操作。
*
* 2024 Copyright(c) Ajay Huajian. All right reserved
**************************************************************/


#include "Infrared_HJ.h"

#define INFRARED_PIN 2  // Infrared的接线引脚
char* isKeyNum; 
InfraredReceiver irReceiver(INFRARED_PIN);

void setup() {
  Serial.begin(115200);
  pinMode(INFRARED_PIN, INPUT);
  digitalWrite(INFRARED_PIN, HIGH);
}

void loop() {
  isKeyNum = irReceiver.getInfraredKey();
  if(strcmp(isKeyNum, "B54AFF00") == 0){//是否按下了按钮9
    //要执行的操作
    Serial.println("按下了按钮9");
  }
}
