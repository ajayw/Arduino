const int buzzerPin = 3;

/*
编号| 1  | 2  | 3  | 4  | 5  | 6  | 7  | 1  |  2  |  3 | 4 |  5 |  6 |  7 
音符 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' , 'D', 'E', 'F', 'G', 'A', 'B'
频率 1976,1760,1568,1492,1320,1175,1046,988,880,784,738,660,587,523
*/
// 定义音调数组，每个元素表示对应的音调频率


// char notes[] = "ccggaagffeeddc "; // 小星星1155665 4433221
// int beats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2};//小星星节拍


char notes[] = "CDECCDECEFGEFGGAGFECGAGFECcgccgc "; // 两只老虎1231 1231 345 345 565431 565431 151 151
float beats[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 0.5, 0.5, 0.5, 0.5, 1, 2, 0.5, 0.5, 0.5, 0.5, 1, 2, 1, 1, 2, 1, 1, 2};//两只老虎节拍

int tempo = 300;

void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2)
  {
    digitalWrite(buzzerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(buzzerPin, LOW);
    delayMicroseconds(tone);
  }
}
void playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' , 'D', 'E', 'F', 'G', 'A', 'B'};
  int tones[] = { 1976,1760,1568,1492,1320,1175,1046,988,880,784,738,660,587,523};
  
  //Plays the tone corresponding to the note name
  for (int i = 0; i < sizeof(tones) / sizeof(tones[0]); i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}

/*****The buzzer plays the little star******/
void sound() 
{
  for (int i = 0; i < sizeof(notes) / sizeof(notes[0]); i++) {
    if (notes[i] == ' ') {
      delay(beats[i] * tempo); // rest
    } else {
      playNote(notes[i], beats[i] * tempo);
    }
    // pause between notes
    delay(tempo / 2); 
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(buzzerPin,OUTPUT);
  sound();
}

void loop() {
  // put your main code here, to run repeatedly:

}

