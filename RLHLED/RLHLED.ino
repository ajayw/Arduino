// 定义引脚（根据实际情况修改引脚号）
const int redLightPin = 5; //红色
const int greenLightPin = 6;//绿色
const int yellowLightPin = 9;//黄色

// 状态定义
enum LightState {OFF, RED, GREEN, YELLOW, RED_BLINK, GREEN_BLINK, YELLOW_BLINK, RED_YELLOW_BLINK, YELLOW_GREEN_BLINK, RED_GREEN_BLINK, ALL_BLINK};
LightState currentState = OFF;

// 计时器变量用于闪烁
unsigned long previousMillis = 0;
const long blinkInterval = 500; // 500ms闪烁间隔
bool lightState = LOW; // 用于闪烁的状态

void setup() {
  // 设置引脚模式
  pinMode(redLightPin, OUTPUT);
  pinMode(greenLightPin, OUTPUT);
  pinMode(yellowLightPin, OUTPUT);

  // 初始化串口
  Serial.begin(9600);
}

void loop() {
  // 检查是否有数据可读取
  if (Serial.available() > 0) {
    byte command = Serial.read();
    handleCommand(command);

    // 回显命令
    Serial.write(command);
  }

  // 更新灯光状态
  updateLights();
}

void handleCommand(byte command) {
  switch (command) {
    case 0x00:
      currentState = OFF;
      break;
    case 0x01:
      currentState = RED;
      break;
    case 0x02:
      currentState = GREEN;
      break;
    case 0x03:
      currentState = YELLOW;
      break;
    case 0x04:
      currentState = RED_BLINK;
      break;
    case 0x05:
      currentState = GREEN_BLINK;
      break;
    case 0x06:
      currentState = YELLOW_BLINK;
      break;
    case 0x07:
      currentState = RED_YELLOW_BLINK;
      break;
    case 0x08:
      currentState = YELLOW_GREEN_BLINK;
      break;
    case 0x09:
      currentState = RED_GREEN_BLINK;
      break;
    case 0x0A:
      currentState = ALL_BLINK;
      break;
    default:
      break; // 未知命令，忽略
  }
}

void updateLights() {
  unsigned long currentMillis = millis();
  
  // 如果是闪烁模式
  if (currentState == RED_BLINK || currentState == GREEN_BLINK || 
      currentState == YELLOW_BLINK || currentState == RED_YELLOW_BLINK || 
      currentState == YELLOW_GREEN_BLINK || currentState == RED_GREEN_BLINK || 
      currentState == ALL_BLINK) {
    if (currentMillis - previousMillis >= blinkInterval) {
      previousMillis = currentMillis;
      lightState = !lightState; // 切换闪烁状态
    }
  }

  // 更新灯光
  switch (currentState) {
    case OFF:
      digitalWrite(redLightPin, LOW);
      digitalWrite(greenLightPin, LOW);
      digitalWrite(yellowLightPin, LOW);
      break;
    case RED:
      digitalWrite(redLightPin, HIGH);
      digitalWrite(greenLightPin, LOW);
      digitalWrite(yellowLightPin, LOW);
      break;
    case GREEN:
      digitalWrite(redLightPin, LOW);
      digitalWrite(greenLightPin, HIGH);
      digitalWrite(yellowLightPin, LOW);
      break;
    case YELLOW:
      digitalWrite(redLightPin, LOW);
      digitalWrite(greenLightPin, LOW);
      digitalWrite(yellowLightPin, HIGH);
      break;
    case RED_BLINK:
      digitalWrite(redLightPin, lightState ? HIGH : LOW);
      digitalWrite(greenLightPin, LOW);
      digitalWrite(yellowLightPin, LOW);
      break;
    case GREEN_BLINK:
      digitalWrite(redLightPin, LOW);
      digitalWrite(greenLightPin, lightState ? HIGH : LOW);
      digitalWrite(yellowLightPin, LOW);
      break;
    case YELLOW_BLINK:
      digitalWrite(redLightPin, LOW);
      digitalWrite(greenLightPin, LOW);
      digitalWrite(yellowLightPin, lightState ? HIGH : LOW);
      break;
    case RED_YELLOW_BLINK:
      digitalWrite(redLightPin, lightState ? HIGH : LOW);
      digitalWrite(greenLightPin, LOW);
      digitalWrite(yellowLightPin, lightState ? LOW : HIGH);
      break;
    case YELLOW_GREEN_BLINK:
      digitalWrite(redLightPin, LOW);
      digitalWrite(greenLightPin, lightState ? HIGH : LOW);
      digitalWrite(yellowLightPin, lightState ? LOW : HIGH);
      break;
    case RED_GREEN_BLINK:
      digitalWrite(redLightPin, lightState ? HIGH : LOW);
      digitalWrite(greenLightPin, lightState ? LOW : HIGH);
      digitalWrite(yellowLightPin, LOW);
      break;
    case ALL_BLINK:
      digitalWrite(redLightPin, HIGH);
      digitalWrite(greenLightPin, LOW);
      digitalWrite(yellowLightPin, LOW);
      delay(500);
      digitalWrite(redLightPin, LOW);
      digitalWrite(greenLightPin, HIGH);
      digitalWrite(yellowLightPin, LOW);
      delay(500);
      digitalWrite(redLightPin, LOW);
      digitalWrite(greenLightPin, LOW);
      digitalWrite(yellowLightPin, HIGH);
      delay(500);
      break;
  }
}
