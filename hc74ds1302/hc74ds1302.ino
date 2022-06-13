#include <DS1302.h>

DS1302 rtc(2, 3, 4); //对应DS1302的RST,DAT,CLK
unsigned char smgduan[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90}; //共阳段码表
//{0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};//显示0~F的值共阴
/****74HC595***/
const byte DS = 11;   //data
const byte ST = 12;  //latch
const byte SH = 10; //clock
unsigned char duanMa[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80}; //选择1-8哪个数码管段
unsigned char duanZhi[8] = {0};
//{0,0,0x40,0,0,0x40,0,0};//共阴
//const long interval = 1000;//设置延时时间间隔
//unsigned long previousMillis = 0;

u8 miao, fen, shi;
//秒、分、时高位低位
u8 miao_L, miao_H, fen_L, fen_H, shi_L, shi_H;

void SendTo595(char byteData)
{
   char i=0;
    for(;i<8;i++)
   {
    digitalWrite(SH,LOW);
    if(byteData & 0x80)digitalWrite(DS, HIGH);     //DS取出最高位（第8位）
    else digitalWrite(DS, LOW);     //DS取出最高位（第8位）
       digitalWrite(SH,HIGH); 
       delayMicroseconds(1);
       byteData <<=1;
   }
}


//void initRTCTime(void)//初始化RTC时钟
//{
//  rtc.writeProtect(false); //关闭写保护
//  rtc.halt(false); //清除时钟停止标志
//  Time t(2021, 9, 26, 21, 50, 50, 7); //新建时间对象 最后参数位星期数据，周日为1，周一为2以此类推
//  rtc.time(t);//向DS1302设置时间数据
//}

void readtime() {
  Time tim = rtc.time(); //从DS1302获取时间数据
  miao = tim.sec; 
  fen = tim.min;
  shi = tim.hr;
}

void display()
{ //先发送8位位码，后发送8位段码
  //8位数码管需要发送8次
  //保存段值/
      digitalWrite(ST, LOW);
  for (int i = 0; i < 8; i++)
  {
     SendTo595(duanMa[i]);
    SendTo595(duanZhi[i]);
//    shiftOut(DS, SH, MSBFIRST, duanMa[i]);////送位码
//    shiftOut(DS, SH, MSBFIRST, duanZhi[i]);//送段码
//      delayMicroseconds(2);
    digitalWrite(ST, HIGH);//ST_CP
    delayMicroseconds(1);
    digitalWrite(ST, LOW);//ST_CP 
  }
}


void update(unsigned char *dst,unsigned char shi,unsigned char fen,unsigned char miao)
{
    Time tim = rtc.time(); //从DS1302获取时间数据
  miao = tim.sec; 
  fen = tim.min;
  shi = tim.hr;
  *dst++ = smgduan[shi/10];
  *dst++ = smgduan[shi%10];
  *dst++ = 0xBF;//0x40共阴-，0xbf共阳-
  *dst++ = smgduan[fen/10];
  *dst++ = smgduan[fen%10];
  *dst++ = 0xBF;
  *dst++ = smgduan[miao/10];
  *dst++ = smgduan[miao%10];
  }

void setup() {
  Serial.begin(9600);

  //新模块上电需要设置一次当前时间，
  //下载完成后需屏蔽此函数再次下载，否则每次上电都会初始化时间数据
  //  initRTCTime();
  pinMode(ST, OUTPUT);//ST_CP
  pinMode(DS, OUTPUT);//DS
  pinMode(SH, OUTPUT);//SH_CP
  digitalWrite(DS, LOW);
//  readtime();
}

void loop() {
 //unsigned long currentMillis = millis();
  while(1){
  update(duanZhi,shi,fen,miao);
  display();
  }

}
