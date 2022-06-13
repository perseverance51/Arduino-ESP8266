/*
    433M无线发射接收RF模块
1、工具-管理库-搜索“RCSwitch”库-安装
2、这是发射端的程序
3、接线说明：
RF433-----ESP8266 NodeMCU
VCC ----- VV (不是VIN，VIN上面没有电压)
GND ----- GND  
DATA -----D2 (GPIO 4)
可调定位器-----ESP8266 NodeMCU
POT----A0
*/

#include <RCSwitch.h>
RCSwitch mySwitch = RCSwitch();

unsigned long val = 0;

void setup() {
    Serial.begin(9600);
    mySwitch.enableTransmit(4);
}
void loop() {
    val = analogRead(A0); 
    Serial.println(val);
    mySwitch.send(val, 23);
    delay(800);
}
