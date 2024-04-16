#include "Arduino.h"

#ifndef ZYMusic_h
#define ZYMusic_h
class HW_BUZZER{
    private:
        //引脚定义
        int _buzzerPin;        //轻触按键连接到Arduino的数字引脚4

    public:
        HW_BUZZER(int micPin);
        PLAYTONE(int tone, float duration);
        PLAYNOTE(char note, float duration);
};
#endif