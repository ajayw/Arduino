#define LEFT_LINE_TRACKING          A0
#define CENTER_LINE_TRACKING        A1
#define right_LINE_TRACKING         A2

void setup()
{
    Serial.begin(9600);
    pinMode(LEFT_LINE_TRACKING, INPUT);
    pinMode(CENTER_LINE_TRACKING, INPUT);
    pinMode(right_LINE_TRACKING, INPUT);
}

void loop()
{
    Infrared_Tracing();
}

void Infrared_Tracing()
{
    int Left_Tra_Value;
    int Center_Tra_Value;
    int Right_Tra_Value;
    Left_Tra_Value = analogRead(LEFT_LINE_TRACKING);
    Center_Tra_Value = analogRead(CENTER_LINE_TRACKING);
    Right_Tra_Value = analogRead(right_LINE_TRACKING);
    Serial.print("Left Tracking value:");
    Serial.println(Left_Tra_Value);
    Serial.print("Center Tracking value:");
    Serial.println(Center_Tra_Value);
    Serial.print("Right Tracking value:");
    Serial.println(Right_Tra_Value);
    Serial.println("");
    delay(1000);
}