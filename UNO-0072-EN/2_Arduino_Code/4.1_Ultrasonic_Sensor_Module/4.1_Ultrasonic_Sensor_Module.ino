// Ultrasonic control pin
const int Trig = 12;
const int Echo = 13;

void setup()
{
    Serial.begin(9600);
    pinMode(Trig, OUTPUT);
    pinMode(Echo, INPUT);
}
void loop()
{
    Ultrasonic_Sensor_Module();
}

void Ultrasonic_Sensor_Module()
{
    int Distance = 0;
    Distance = getDistance();
    Serial.print("Distance:");
    Serial.print(Distance);
    Serial.println("CM");
    delay(100);
}

float getDistance()
{
    digitalWrite(Trig, LOW);
    delayMicroseconds(2);
    digitalWrite(Trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(Trig, LOW);
    float distance = pulseIn(Echo, HIGH) / 58.00;
    delay(10);
    return distance;
}


