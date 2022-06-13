#include <Arduino.h>
#define Button (5)  //按键的引脚号

static  unsigned char Trg=0;
static unsigned char Cont=0;
static  unsigned char CNT=0;

//bool KeyValue = digitalRead(Button);
void KeyRead()
{
    unsigned char ReadData = digitalRead(Button)^0x01;   // 
    Trg = ReadData & (ReadData ^ Cont);   // 按键触发为1
    Cont = ReadData;                     // 长按触发为1
  
}
void setup()
{
  pinMode(Button, INPUT);  //按键设置为输入模式
  digitalWrite(Button, HIGH);
  Serial.begin(115200); //波特率设置

}

void loop()
{
//
 while(1){
    KeyRead();
 bool  ReadData = ~digitalRead(Button);
   if(Trg && ReadData ) {
Serial.println("按键触发");
CNT++;
Serial.println(CNT);
}
 } 
}
