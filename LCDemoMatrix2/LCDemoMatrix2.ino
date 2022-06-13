/***Arduino UNO+ MAX7219驱动8X8点阵显示***/
/*
使用pctolcd2002取模软件
图像取模：8X8点阵
取模注意，需要右移一个像素
 pin 11 is connected to the DataIn 
 pin 10 is connected to the CLK 
 pin 9 is connected to LOAD 
 We have only a single MAX72XX.
 */
#include <LedControl.h>
 
int DIN = 11;
int CS =  10;
int CLK = 9;
 

byte eight[8]= {0x7E,0x7E,0x66,0x7E,0x7E,0x66,0x7E,0x7E};  //8

 
LedControl lc=LedControl(DIN,CLK,CS,1);
 
void setup(){
 lc.shutdown(0,false);       //启动时，MAX72XX处于省电模式
 lc.setIntensity(0,8);       //将亮度设置为最大值
 lc.clearDisplay(0);         //清除显示
}
 
void loop(){ 
    byte smile[8]=   {0x3C,0x42,0xA5,0x81,0xA5,0x99,0x42,0x3C};//笑脸
    byte neutral[8]= {0x3C,0x42,0xA5,0x81,0xBD,0x81,0x42,0x3C};//标准脸    

byte chu[8]={0x08,0x49,0x49,0x7F,0x08,0x49,0x49,0x7F};//选列"出"
byte lu[8]={0x40,0x20,0x10,0x08,0x0C,0x12,0x21,0xC0};//"入"
byte pin[8]={0xFF,0xC8,0x29,0x1A,0xFF,0x08,0x08,0x08};//"平"
//byte pin[8]={0xFF,0x4A,0x2A,0x1C,0xFF,0x08,0x08,0x08};//"平"
byte an[8]={0x10,0xFF,0xD4,0xFF,0x14,0x1C,0x22,0x41};//"安"
//{0x08,0xFF,0x89,0x04,0x7F,0x14,0x1C,0x63};//"安"

    printByte(eight);//显示8
    delay(1000);//延时1秒
    printByte(chu);//显示标准脸
    delay(1000);
        printByte(lu);//显示标准脸
    delay(1000);
        printByte(pin);//"平"
    delay(1000);
        printByte(an);//"安"
    delay(1000);
}
 
//点阵显示函数
void printByte(byte character [])
{
  int i = 0;
  for(i=0;i<8;i++)
  {
    lc.setRow(0,i,character[i]);
  }
}
