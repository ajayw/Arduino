#include <Adafruit_NeoPixel.h>  //RGB libraries
#include <dht.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <IRremote.h>           //Infrared libraries
#include <SPI.h> 
#include <RFID.h>
#include <Servo.h>              // Servo libraries
RFID rfid(10, 9);               //D10: the pin of the tag reader SDA. D9: Label reader RST pin

unsigned char str[MAX_LEN];     //MAX_LEN is 16: size of the array 

String accessGranted [2] = {"210101368113", "96111131033"};  //Rfid serial number authorized access
int accessGrantedSize = 2;                                  //Number of serial numbers
Servo lockServo;                //Locking mechanism servo
int lockPos = 10;                //Lock position limitation
int unlockPos =90;              //Limit of unlock position
boolean locked = true;
int rain;     
Servo windowServo;  // Create a servo object to control the servo
int pos = 0;        // Variables store the servo position
int windowFlag = 0;   

dht DHT;
#define NUMPIXELS     4           //Number of LED lamp beads
#define RGB_PIN       5           //RGB module Arduino output control signal pin
#define LED_PIN       8           //LED module Arduino output control signal pin
#define KEY_1         A6          //Touch button Arduino output control signal pin A6
#define KEY_2         A7          //Yellow button module Arduino output control signal pin
#define buzzer        3           //Buzzer module Arduino output control signal pin
#define smokeA0       A0          //MQ-2 gas sensor Arduino output control signal pin
#define DHT11_PIN     4           //DHT11 temperature and humidity Arduino output control signal pin
#define IR_PIN        2           //Infrared receiver Arduino output control signal pin
#define RelayPin      16          //Relay module Arduino output control signal pin    
#define FanPinA       7           //Fan motor module Arduino output control signal pin
#define rain_Pin      A3          //Raindrop sensor Arduino output control signal pin

#define KEY11 16753245    //btn1:Control LCD to turn on the backlight
#define KEY22 16736925    //btn2:Control LCD to turn off backlight
#define KEY33 16769565    //btn3:Control fan start
#define KEY44 16720605    //btn4:Control fan off
#define KEY55 16712445    //btn5:Control window opening
#define KEY66 16761405    //btn6:Control window closing
#define KEY77 16769055    //btn7:Control door open
#define KEY88 16754775    //btn8:Control door closing 
#define KEY00 16750695    //btn0:Conrrol open automation
#define KEY1 16718055     //btnUp:Open white RGB
#define KEY2 16730805     //btnDown:Open red RGB
#define KEY3 16716015     //btnLeft:Open green RGB
#define KEY4 16734885     //btnRight:Open blue RGB
#define KEY5 16726215     //btnOK:Closing RGB

//Set the LCD address to 0x27 to display in 16 characters and 2 lines

LiquidCrystal_I2C lcd(0x27,20,4);
IRrecv irrecv(IR_PIN);
decode_results results;
Adafruit_NeoPixel pixels(NUMPIXELS, RGB_PIN, NEO_GRB + NEO_KHZ800);//Create light objects

//Variable definitions
int count_R=0;
int count_G=0;
int count_B=0;
int count_key=0;
int Press=0;              //Defines key flag bits
int rotate=0;
int KEY_BIT=0;            //Defining status bits
int LED_BIT=0;            //Defining status bits
int sensorThres = 100;     //Set the gas concentration range
// int count_key = 0;      
int door = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(KEY_1,INPUT);     //Touch definition input
	pinMode(KEY_2,INPUT);     //Key definition input
  pinMode(LED_PIN,OUTPUT);  //LED
	pixels.begin();           //Initialize 2812 library functions
	pixels.show();
  pinMode(buzzer, OUTPUT);  //The buzzer defines the output
  pinMode(smokeA0, INPUT);  //Smoke definition input
  pinMode(FanPinA, OUTPUT); //fan
  pinMode(RelayPin,OUTPUT); //Relay module
  pinMode(DHT11_PIN, INPUT);
  windowServo.attach(6);        // Connect the servo on pin 6 to the servo object
  windowServo.write(130);
  pinMode(rain_Pin, INPUT);     
  digitalWrite(RelayPin, LOW);
  digitalWrite(FanPinA, LOW); 
  
  Serial.println("DHT TEST PROGRAM ");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT_LIB_VERSION);
  Serial.println();
  Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)");

  lcd.init();   // Initialized liquid crystal
  // Print message to LCD
  lcd.backlight();
  irrecv.enableIRIn();    // Starting receiver
	Serial.begin(9600);
  SPI.begin();            //Start SPI communication with the reader
  rfid.init();            //initialize 
  lockServo.attach(15);
  lockServo.write(lockPos);         //Move the servo to the locked position
  Serial.println("Place card/tag near reader...");
}

void loop() {
  // put your main code here, to run repeatedly:
  pixels.clear();   //Backlight
  KEY();            //The key function
	RGB();            //RGB
  LED();            //LED
  rainwater();      //Raindrop sensor
  RFID();             //RFID
  smoke();
  LCD();
}

//The touch key initializes the function

void KEY()   //Key Function
{
  int i=analogRead(KEY_1);//A6引脚使用读取模拟方法获取触摸模块数值    Pin A6 uses the read simulation method to obtain the touch module value
	if((i<300)&&(Press==0)){  
    	Press=1;                //按下标志==1   Press the sign
	}
  i=analogRead(KEY_1);
  if((i>1000) &&(Press==1))   //松开触控按钮且按下标志==1   Let go detection
  {
    // Serial.print("i:");
    // Serial.println(i);
    // Serial.print("count_key:");
    // Serial.println(count_key);
    Press=0;
    count_key++;
    if(count_key>5)count_key=1;//超过5次回到初始值1   Return to the initial value of 1 more than 5 times
  }
}

//LED function
void LED()
{
  if((analogRead(KEY_2))&&(KEY_BIT)) //Determine if the key is pressed
  {
    KEY_BIT=0;
    if(LED_BIT==0)LED_BIT=1;
    else LED_BIT=0;
  }else{
    delay(20);
    if(!analogRead(KEY_2))
    {
      KEY_BIT=1;
    }
  }
  if(LED_BIT)digitalWrite(LED_PIN,HIGH);  //MCU output high level
  else digitalWrite(LED_PIN,LOW);         //MCU output low level
}

//RGB function
void RGB()
{
  switch(count_key)    //Number of keys 
  {
    case 1 :            //The red light is on.
      pixels.setPixelColor(0, pixels.Color(255, 0, 0));
      pixels.setPixelColor(1, pixels.Color(255, 0, 0));
      pixels.setPixelColor(2, pixels.Color(255, 0, 0));
      pixels.setPixelColor(3, pixels.Color(255, 0, 0));
      pixels.show();
      break;
    case 2 :              //green light flashes
      pixels.setPixelColor(0, pixels.Color(0, 255, 0));
      pixels.setPixelColor(1, pixels.Color(0, 255, 0));
      pixels.setPixelColor(2, pixels.Color(0, 255, 0));
      pixels.setPixelColor(3, pixels.Color(0, 255, 0));
      pixels.show();
      break;
    case 3 :               //The blue lights
      pixels.setPixelColor(0, pixels.Color(0, 0, 255));
      pixels.setPixelColor(1, pixels.Color(0, 0, 255));
      pixels.setPixelColor(2, pixels.Color(0, 0, 255));
      pixels.setPixelColor(3, pixels.Color(0, 0, 255));
      pixels.show();
      break;
    case 4 :           //bright white light
        //delay(100);
        pixels.setPixelColor(0, pixels.Color(255, 255, 255));
        pixels.setPixelColor(1, pixels.Color(255, 255, 255));
        pixels.setPixelColor(2, pixels.Color(255, 255, 255));
        pixels.setPixelColor(3, pixels.Color(255, 255, 255));
        pixels.show();
        break;
    case 5 :        // Turn our the lights
      pixels.setPixelColor(0, pixels.Color(0, 0, 0));
      pixels.setPixelColor(1, pixels.Color(0, 0, 0));
      pixels.setPixelColor(2, pixels.Color(0, 0, 0));
      pixels.setPixelColor(3, pixels.Color(0, 0, 0));
      pixels.show();
      break;
     default : break;
  }
}

void rainwater()
 {
    // Serial.print("RainData =");
    // Serial.println(rain);
    if(windowFlag == 1){
        //Sensor data
        rain = analogRead(rain_Pin);
        if(rain > 800){
          windowServo.write(130); //close the door
        }else{
          windowServo.write(10);  //open the door
        }
    }
}

//RFID
void RFID()
{
  if (rfid.findCard(PICC_REQIDL, str) == MI_OK)   //Wait for a tag to be placed near the reader
  {
    Serial.println("Card found");
    String temp = "";                             //A temporary variable that stores the READ RFID number
    if (rfid.anticoll(str) == MI_OK)              //Anti-collision detection, read the label serial number
    {
      Serial.print("The card's ID number is : ");
      for (int i = 0; i < 4; i++)                 //Record and display the label serial number
      {
        temp = temp + (0x0F & (str[i] >> 4));
        temp = temp + (0x0F & str[i]);
      }
      Serial.println (temp);
      checkAccess (temp);     //Check whether the identified label is allowed to be opened
    }
    rfid.selectTag(str); //Lock the card to prevent repeated reading, removing the line will cause the sketch to read the card continuously
  }
  rfid.halt();
}

void checkAccess (String temp)    //The function checks whether a registered tag is accessible
{
  boolean granted = false;
  for (int i=0; i <= (accessGrantedSize-1); i++)    //Iterates over all tag ID numbers registered in the group
  {
    if(accessGranted[i] == temp)            //If a label is found, then open/close the lock
    {
      Serial.println ("Access Granted");
      
      granted = true;
      if (locked == true)         //If the lock is closed, open it
      {
          lockServo.write(unlockPos);
          locked = false;
      }
      else if (locked == false)   //If the lock is open, close it
      {
          lockServo.write(lockPos);
          locked = true;
      }
    }
  }
  if (granted == false)     //If no tags are found
  {
    Serial.println ("Access Denied");
  }
}

//Mq-2 gas function
void smoke()
{
  int analogSensor = analogRead(smokeA0); //Assign the check value of the MQ-2 sensor to the ALOG sensor variable

  // Serial.print("Pin A0: ");
   Serial.println(analogSensor);
  //Check if the threshold has been reached
  if (analogSensor > sensorThres)
  {
    tone(buzzer, 1000, 200);              //蜂鸣器响    The buzzer sounded
    digitalWrite(RelayPin,HIGH);  //High output; Relay start
  }else{
    noTone(buzzer);                       //蜂鸣器停    The buzzer stops
    digitalWrite(RelayPin,LOW);
  }
}

//Infrared control LCD 1602, fan, door and window, LED function
void LCD()
{
   translateIR();  //Infrared controls RGB functions
   ShowHumiture();
    if (irrecv.decode(&results)) 
    {
        //Serial.print("value = ");
        //Serial.println(results.value, HEX);
        if (results.value == KEY11 )     // LCD boot
        {
          lcd.backlight(); //Turn on the backlight
        } 
        else if (results.value == KEY22 ) //LCD OFF
        {
            lcd.noBacklight(); // Turn off the backlight
            lcd.clear();
        } 
          if (results.value == KEY33 ) //Switch on and activate the relay
        {
           digitalWrite(RelayPin, HIGH);
           digitalWrite(FanPinA, HIGH);
        } 
        else if (results.value == KEY44 ) // Disconnect the relay
        {
            digitalWrite(RelayPin, LOW);
            digitalWrite(FanPinA, LOW);
        } 

       if (results.value == KEY55 )   //Open Window
        {
          windowFlag = 0;
          windowServo.write(10);
          delay(15);
        } 
        else if (results.value == KEY66 ) //close windows
        {
          windowFlag = 0;
          windowServo.write(130);  //Change the degree of rotation of the steering gear
          delay(15);
        }    

        if (results.value == KEY77 )         //Open the door
          {   lockServo.write(0);
              delay(15);
           }
       else if (results.value == KEY88 )     //Close the gate
       {
          lockServo.write(90);
          delay(15);
       }

        if (results.value == KEY00 )         //Conrrol open automation
        {
          windowFlag = 1;
        }
        irrecv.resume(); // Receive the next value
      }
      delay(100);
 }

//Display temperature
void ShowHumiture()
{
   int chk = DHT.read11(DHT11_PIN);
    //Serial.println("DHT TEST PROGRAM ");
    lcd.setCursor(0,0);  
    lcd.print("humi:");
    lcd.setCursor(5,0);
    lcd.print(DHT.humidity);

    lcd.setCursor(0,1); 
    lcd.print("temp:");
    lcd.setCursor(5,1);
    lcd.print(DHT.temperature);
}
//Infrared control RGB 
void translateIR() // Act on received IR code
{
  switch(results.value)
  {
    case 0xFF4AB5:    count_key=1; break;   //Red
    case 0xFF10EF:    count_key=2; break;   //green
    case 0xFF5AA5:    count_key=3; break;   //blue
    case 0xFF18E7:    count_key=4; break;   //white
    case 0XFF38C7:    count_key=5; break;   //Hit the OK button to turn off the lights
    default: break;
  }
} 
