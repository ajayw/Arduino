#ifndef _Infrared_HJ_H
#define _Infrared_HJ_H
#include "Arduino.h"

class InfraredReceiver {
  public:
    InfraredReceiver(int pin);
    char* getInfraredKey();

  private:
    int _INFRARED_PIN;
    unsigned long pulseTime;
    byte IrValue[4];    //用户码、用户反码、数据码和数据反码
    char getKeyNum[9];  //为了确保结果是8个字符长度，我们将结果存储在一个长度为9的字符数组中（包括最后一个空字符）
};

#endif