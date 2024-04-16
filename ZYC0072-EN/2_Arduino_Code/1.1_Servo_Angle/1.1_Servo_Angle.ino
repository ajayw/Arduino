#include    <Servo.h>

#define CLAW_PIN    11
#define ARM_PIN     10
#define BASE_PIN    9

Servo clawservo;//Clamp servo
Servo armservo;//Arm servo
Servo baseservo;//Turntable servo

int pos = 0;

void setup()
{
    clawservo.attach(CLAW_PIN);
    armservo.attach(ARM_PIN);
    baseservo.attach(BASE_PIN);
    clawservo.write(135);
    armservo.write(90);
    baseservo.write(90);
}

void loop()
{
    for (pos = 135; pos >= 45; pos -= 1)
    { 
        clawservo.write(pos);    
        delay(15);
    } 
    for (pos = 45; pos <= 135; pos += 1)
    { 
        clawservo.write(pos);            
        delay(15);                     
    }
    
    for (pos = 90; pos <= 170; pos += 1)
    { 
        armservo.write(pos);            
        delay(15);                     
    }

    for (pos = 170; pos >= 90; pos -= 1)
    { 
        armservo.write(pos);    
        delay(15);
    }
    
    for (pos = 90; pos >= 0; pos -= 1)
    { 
        baseservo.write(pos);    
        delay(15);
    }
    for (pos = 0; pos <= 180; pos += 1)
    { 
        baseservo.write(pos);            
        delay(15);                     
    }
    for (pos = 180; pos >= 90; pos -= 1)
    { 
        baseservo.write(pos);    
        delay(15);
    }

}
