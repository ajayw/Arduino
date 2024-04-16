#include "ZYMusic.h"

HW_BUZZER::HW_BUZZER(int micPin){
    pinMode(micPin, OUTPUT);                 //将按键引脚设置为输入模式并使用内部上拉电阻
    _buzzerPin = micPin;
}

HW_BUZZER::PLAYTONE(int tone, float duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2)
  {
    digitalWrite(_buzzerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(_buzzerPin, LOW);
    delayMicroseconds(tone);
  }
}

HW_BUZZER::PLAYNOTE(char note, float duration){
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' , 'D', 'E', 'F', 'G', 'A', 'B'};
  int tones[] = { 1976,1760,1568,1492,1320,1175,1046,988,880,784,738,660,587,523};
  
  //Plays the tone corresponding to the note name
  for (int i = 0; i < 14; i++) {
    if (names[i] == note) {
      PLAYTONE(tones[i], duration);
    }
  }
}
