#include "Infrared_HJ.h"

InfraredReceiver::InfraredReceiver(int pin) {
  _INFRARED_PIN = pin;
}

char* InfraredReceiver::getInfraredKey() {
  /*
  *检查是否接收到起始脉冲，低电平持续9ms左右，高电平持续4.5ms；
  *第一个while,10su乘上1000，约10ms；
  *第二个while,10us乘上500，约5ms；
  */
  if (digitalRead(_INFRARED_PIN) == 0) {
    pulseTime = 1000;
    int dataNumber = 0;
    while ((digitalRead(_INFRARED_PIN) == 0) && (pulseTime > 0))  //低电平持续10ms
    {
      delayMicroseconds(10);
      pulseTime--;
    }
    if (digitalRead(_INFRARED_PIN) == 1) {
      int i, j;
      pulseTime = 500;
      while ((digitalRead(_INFRARED_PIN) == 1) && (pulseTime > 0)) {//高电平持续5ms
        delayMicroseconds(10);
        pulseTime--;
      }
      Serial.println();
      /*----------------------------------------------------------------------
        *嵌套for循环，第一个i<4,代表重复获取4个码，用户码、用户反码、数据码、数据反码；
        *用户码和用户反码默认0和255，所以得到的十六进制00FF；
        *每一个数据码8位，所以j<8,数据码阶段高电平持续时间决定是"位0"还是"位1",8个0或者1组成一个数就是该数据码；
        */
      for (i = 0; i < 4; i++) {
        for (j = 0; j < 8; j++) {
          pulseTime = 60;
          while ((digitalRead(_INFRARED_PIN) == 0) && (pulseTime > 0)) {
            delayMicroseconds(10);
            pulseTime--;
          }
          pulseTime = 500;
          while ((digitalRead(_INFRARED_PIN) == 1) && (pulseTime > 0)) {
            delayMicroseconds(100);
            dataNumber++;
            pulseTime--;
            if (dataNumber > 30) {
              break;            //超时没有接收到信号就跳出，结束信号解析
            }
          }
          IrValue[i] >>= 1;     //要从低位开始，先移一位再存值
          if (dataNumber >= 8)  //大于0.8ms认为是1，否则为0
          {
            IrValue[i] |= 0x80;
          }
          dataNumber = 0;
        }
      }
    }
    if (IrValue[2] + IrValue[3] == 255) {
      sprintf(getKeyNum, "%02X%02X%02X%02X", IrValue[3], IrValue[2], IrValue[1], IrValue[0]);
      Serial.println(getKeyNum);
      return getKeyNum;
    }
  }
}