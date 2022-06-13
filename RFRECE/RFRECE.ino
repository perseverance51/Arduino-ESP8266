/*

1、工具-管理库-搜索“RCSwitch”库-安装
2、发射模块
VCC GND  DATA- arduino 11脚
3、接收模块
VCC GND  DATA- arduino 2脚
*/

#include <RCSwitch.h>
RCSwitch myFa = RCSwitch();//定义发送端
RCSwitch myShou = RCSwitch();//定义接收端
unsigned long i = 0; //注意，这里的数据类型，不能用int

void setup(){
  myFa.enableTransmit(11);//发送端接6号口（或其它口）
  myShou.enableReceive(0);//接收端接中断0（中断0为Arduino的2口）
 Serial.begin(9600);//打开串口调试
}

void loop(){
 i = i+1;
 myFa.send(i, 24);//发送端发送数据，i为数字
 if (myShou.available()) {
   int value = myShou.getReceivedValue();//接收端获取数据
   if (value != 0) {
     Serial.println(i);
   }
   myShou.resetAvailable();//接收端重设活动状态
 }
 delay(1000);
}
