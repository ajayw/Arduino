# Infrared_HJ
## 这是一个适用于UNO、Nano、UNO R4 Minima等Arduino的红外遥控库。
由于原来的红外遥控的库在用于Minima板的时候会乱码(在4.2.1之后已经可以使用，但与其他板又不兼容了)，而且很多版本的接收都不稳定，所以根据红外接收器硬件特性波形重新尝试写的，通过简单解码即可得到键码，再通过“和”校验加上用户码得到红外遥控器的键码。不一定是最正确高效的，但整体代码简单，键码能正常接收，欢迎使用并提出意见建议。

## 使用方法：
1、创建实例对象：InfraredReceiver irReceiver(INFRARED_PIN);//接收参数为红外接收器连接的引脚，一般是D2 

2、波特率设置115200：Serial.begin(115200);

3、调用对象函数（可以保存到字符数组指针）:isKeyNum = irReceiver.getInfraredKey();

（其他参照代码样例）
