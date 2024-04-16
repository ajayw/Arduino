
#include <SPI.h>              //SPI通信   SPI communication
#include <RFID.h>             //添加读卡器库    Add a card reader library
#include <Servo.h>            //添加舵机库    Add servo library

RFID rfid(10, 9);             //读卡器SDA连接引脚10，读卡器RST连接引脚9   D10:pin of tag reader SDA. D9:pin of tag reader RST
unsigned char str[MAX_LEN];   //字符数   MAX_LEN is 16: size of the array   eg：[42,173,104,179]  101010,10101101,1101000,10110011

String accessGranted [2] = {"210101368113", "31215123949"}; //用于授权访问的RFID序列号    RFID serial numbers to grant access to
int accessGrantedSize = 2;                                  //序列号的数量    The number of serial numbers

Servo lockServo;                    //门锁装置舵机    Servo for locking mechanism
int lockPos = 10;                   //锁定位置角度    Locked position limit 
int unlockPos =90;                  //无锁位置角度    Unlocked position limit
boolean locked = true;

void setup()
{
  Serial.begin(9600);               //串行监视器只需要获取标签ID号和用于故障排除    Serial monitor is only required to get tag ID numbers and for troubleshooting
  SPI.begin();                      //启动与reader的SPI通信   Start SPI communication with reader
  rfid.init();                      //初始化    initialization
  lockServo.attach(A1);
  lockServo.write(lockPos);         //将伺服移至锁定位置    Move servo into locked position
  Serial.println("Place card/tag near reader...");
}

void loop()
{ 
  if (rfid.findCard(PICC_REQIDL, str) == MI_OK)   //等待标签放置在读卡器附近    Wait for a tag to be placed near the reader
  {
    Serial.println("Card found"); 
    String temp = "";                             //存储读取的RFID编号的临时变量    Temporary variable to store the read RFID number
    if (rfid.anticoll(str) == MI_OK)              //防碰撞检测，读取标签序列号    Anti-collision detection, read tag serial number 
    {
      Serial.print("The card's ID number is : ");
      for (int i = 0; i < 4; i++)                 //记录并显示标签序列号  Record and display the tag serial number
      {
        //Serial.println(str[i]);                   //10 1010,1010 1101,110 1000,1011 0011   高、低四位转换十进制：2 10,10 13,6 8,11 3 拼接后得到 210101368113
        temp = temp + (0x0F & (str[i] >> 4));     //右移4位后位与运算取高四位再与temp拼接成新的字符串   Shift 4 bits to the right and take 4 bits higher and then splice temp into a new string
        temp = temp + (0x0F & str[i]);            //直接位与运算取低四位再与temp拼接成新的字符串    The direct AND takes the lower four bits and concatenates them with temp to form a new string
      }
      Serial.println (temp);
      checkAccess (temp);                         //检查识别的标记是否为允许打开的标记    Check if the identified tag is an allowed to open tag
    }
    rfid.selectTag(str);                          //锁卡为防止冗余读，删除线条将使素描卡连续读  Lock card to prevent a redundant read, removing the line will make the sketch read cards continually
  }
  rfid.halt();
}

void checkAccess (String temp)                    //函数，检查已识别的标记是否已注册以允许访问  Function to check if an identified tag is registered to allow access
{
  boolean granted = false;
  for (int i=0; i <= (accessGrantedSize-1); i++)  //遍历数组中注册的所有标签ID号  Runs through all tag ID numbers registered in the array
  {
    if(accessGranted[i] == temp)                  //如果找到标记，则打开/关闭锁 If a tag is found then open/close the lock
    {
      Serial.println ("Access Granted");
      granted = true;
      if (locked == true)                         //如果锁是关着的，就打开它  If the lock is closed then open it
      {
          lockServo.write(unlockPos);
          locked = false;
      }
      else if (locked == false)                   //如果锁是开着的，就关上它  If the lock is open then close it
      {
          lockServo.write(lockPos);
          locked = true;
      }
    }
  }
  if (granted == false)                           //如果没有找到标签  If the tag is not found
  {
    Serial.println ("Access Denied");
  }
}
