#include <Wire.h> 
#include <LiquidCrystal_I2C.h>  //添加LCD显示屏库   Add LCD display library
//设置LCD地址为0x27，以实现16字符和2行显示   set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27,20,4); 

void setup()
{
  Serial.begin(9600);
  lcd.init();         //初始化lcd   initialize the lcd 
  lcd.backlight();    //开启LCD背光 open the backlight
  // lcd.noBacklight();  //关闭LCD  Close the backlight
  lcd.clear();        //清除内容    clear LCD
}
void loop()
{
    /*
    *lcd.setCursor(x,y);  //设置第y行，第x个字符开始显示    Set line y and the XTH character to be displayed
    *lcd.print("number:");//设置要显示的内容    Set the content to display
    */
    lcd.setCursor(0,0);   //从第1个字符开始显示number：  Displays the "number:" starting from the first character
    lcd.print("number:");
    lcd.setCursor(7,0);   //从第7个字符开始显示123    Display 123 starting at the seventh character
    lcd.print("123");     //在第一行显示数值    Displaying number

    lcd.setCursor(0,1);
    lcd.print("char:");
    lcd.setCursor(5,1);
    lcd.print("ABC");    //在第二行显示字符    Displaying characters
    delay(100);
}
