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
RCSwitch mySwitch = RCSwitch();
int pos = 0;
void setup() {
    Serial.begin(9600);
    pinMode(4,OUTPUT);
    pinMode(5,OUTPUT);
    pinMode(8,OUTPUT);
    pinMode(9,OUTPUT); 
    mySwitch.enableReceive(0);
}
void loop() {
    if (mySwitch.available()) {
        int value = mySwitch.getReceivedValue();
        if (value > 0) {
            if (mySwitch.getReceivedValue()==1){digitalWrite(4,HIGH);}
            else if (mySwitch.getReceivedValue()==2){digitalWrite(5,HIGH);}
            else if (mySwitch.getReceivedValue()==3){digitalWrite(8,HIGH);}
            else if (mySwitch.getReceivedValue()==4){digitalWrite(9,HIGH);}
            else if (mySwitch.getReceivedValue()==5){
                digitalWrite(4,LOW);
                digitalWrite(5,LOW);
                digitalWrite(8,LOW);
                digitalWrite(9,LOW);
            }
        }
        mySwitch.resetAvailable();
    }
}
