#define  Buzzer  3      // 定义引脚3为Buzzer  Define output pin
#define  KEY  A7        // 定义A7引脚为KEY  Define input pin

bool buttonState=1;     //定义按钮状态  Define key status bits
bool lastButtonState=1; //前一刻按键状态  Last key press status

void setup ()           //设置函数  Function initialization
{
	pinMode(Buzzer,OUTPUT);  //定义LED引脚为输出  Define output
	pinMode(KEY,INPUT);   //定义KEY引脚为输入  Define input
}

void loop()   //Principal function
{
  buttonState = analogRead(KEY);          //读取按键状态  Reading key state (A7引脚只支持读取模拟值不支持读数字值)
  if (buttonState != lastButtonState) {   //检测按键状态是否发生改变  Detect if the key state has changed
    delay(50);                            //等待一段时间以避免按键抖动  Wait a while to avoid key jitter
    buttonState = analogRead(KEY);        //再次读取按键状态进行确认  Read the key state again for confirmation
    if (buttonState != lastButtonState) { //检测确认后的按键状态  Check the status of the key after confirmation
      if (buttonState == LOW) {           //按键被按下  The key is pressed
        tone(Buzzer,523);                 //蜂鸣器响  The buzzer sounded
      }else{
        noTone(Buzzer);                   //蜂鸣器停  The buzzer stops
      }
    }
  }
  lastButtonState = buttonState;      //更新上一次按键状态
}
