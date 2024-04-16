/*
  ZY-Music.ino v1.7.6
  Author: Ajay Huajian
  2023 Copyright(c) ZHIYI Technology Inc. All right reserved

  编号| 1  | 2  | 3  | 4  | 5  | 6  | 7  | .1 | .2 | .3 | .4 | .5 | .6 | .7 
  音符 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' , 'D', 'E', 'F', 'G', 'A', 'B'
*/

#include "ZYMusic.h"
int Pin_buzzer = 3;   //蜂鸣器连接引脚3

HW_BUZZER HW508(Pin_buzzer);//实例化蜂鸣器对象
char notes[] = "CDECCDECEFGEFGGAGFECGAGFECCgCCgC "; // 两只老虎1231 1231 345 345 565431 565431 151 151
float beats[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 0.5, 0.5, 0.5, 0.5, 1, 1, 0.5, 0.5, 0.5, 0.5, 1, 1, 1, 1, 2, 1, 1, 2};//两只老虎节拍
int tempo = 300;
void setup() {
  Serial.begin(9600);
  pinMode(Pin_buzzer, OUTPUT);
  sound();//在这里每次重启只执行一次音乐播放，要重复播放请在loop()中打开sound函数调用
}

void sound(){
  for (int i = 0; i < sizeof(notes) / sizeof(notes[0]); i++) {
    if (notes[i] == ' ') {
      delay(beats[i] * tempo); // rest
    } else {
      HW508.PLAYNOTE(notes[i], beats[i] * tempo);
    }
    // pause between notes
    delay(tempo / 2);
  }
}
void loop() {
  //调用sound函数
  sound();
}

