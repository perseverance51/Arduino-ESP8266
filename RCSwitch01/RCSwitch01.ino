/*
    433M无线发射接收RF模块
1、工具-管理库-搜索“RCSwitch”库-安装
2、这是发射端的程序
3、接线说明：
RF433-----ESP8266 NodeMCU
VCC ----- VV (不是VIN，VIN上面没有电压)
GND ----- GND  
DATA -----D2 (GPIO 4)

*/

#include <RCSwitch.h>
RCSwitch myFa = RCSwitch();//定义发送端
//RCSwitch myShou = RCSwitch();//定义接收端
unsigned long i = 0; //注意，这里的数据类型，不能用int

void setup(){
      pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);//板子led，作为观察数据发射状态
  myFa.enableTransmit(4);//D2 使能发送端口（或其它口）
//  myShou.enableReceive(0);//接收端接中断0（中断0为Arduino的2口）
 Serial.begin(9600);//打开串口调试
}

void loop(){
 i = i+1;
 myFa.send(i, 24);//发送端发送数据，i为数字，数据长度24.
 digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
 i %= 1024;//i累加到1024归零
 delay(800);
}
