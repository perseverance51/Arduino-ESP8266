/*
      433M无线发射接收RF模块
1、工具-管理库-搜索“RCSwitch”库-安装
2、这是接收端的程序
3、接线说明：
 
RF433-----ESP8266 NodeMCU
VCC ----- VV (不是VIN，VIN上面没有电压)
GND ----- GND  
DATA -----D7 (GPIO 13)
*/

#include <RCSwitch.h>
//RCSwitch myFa = RCSwitch();//定义发送端
RCSwitch myShou = RCSwitch();//定义接收端
unsigned long NUM = 0; //注意，这里的数据类型，不能用int
bool flag = false;
void setup(){
    pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);//板子led，作为观察数据接收状态
  myShou.enableReceive(13);//D7 接收端接中断
 Serial.begin(9600);//打开串口调试
}

void loop(){
    while(myShou.available()) {
   NUM =  myShou.getReceivedValue();//接收端获取数据
   delay(5);
   flag = true;//接收到数据标志位
   myShou.resetAvailable();//接收端重设活动状态
  }
   if (flag == true) {//标志位判断
    Serial.print("Message: ");
     Serial.println(NUM);
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));//板子led状态改变
     flag = false;//清除标志位
//     buf = "";//使用完数据，清空变量的数据，等待下一次接收
     //  memset(buf, 0, sizeof(buf));
    //  strcpy(buf, "");
   }  
 }
