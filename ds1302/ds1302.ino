#include <DS1302.h>

DS1302 rtc(2, 3, 4); //对应DS1302的RST,DAT,CLK
unsigned char smgduan[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90}; //共阳段码表
//{0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};//显示0~F的值共阴
/****74HC595***/
const byte DS = 11;   //data
const byte ST = 12;  //latch
const byte SH = 10; //clock
char weiMa[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80}; //选择1-8哪位数码管段
char duanZhi[8] = {0, 0, 0xBF, 0, 0, 0xBF, 0, 0};
//{0,0,0x40,0,0,0x40,0,0};//共阴
const long interval = 1000;//设置延时时间间隔
unsigned long previousMillis = 0;
static unsigned char i =0;
//u16 miao, fen, shi;
//秒、分、时高位低位
u8 miao_L, miao_H, fen_L, fen_H, shi_L, shi_H;

//void SendTo595(char byteData)
//{
//   char i=0;
//    for(;i<8;i++)
//   {
//        digitalWrite(DS, byteData>>7);     //DS取出最高位（第8位）
//        byteData= byteData<<1;  //将第7位移动到最高位
//       digitalWrite(SH,LOW);        //变为低电平，为下次准备  ,并延时2个时钟周期
//        delayMicroseconds(2);
//       digitalWrite(SH,HIGH);          //上升沿，让串行输入时钟变为高电平，
//   }
//}


//void initRTCTime(void)//初始化RTC时钟
//{
//  rtc.writeProtect(false); //关闭写保护
//  rtc.halt(false); //清除时钟停止标志
//  Time t(2021, 9, 26, 21, 50, 50, 7); //新建时间对象 最后参数位星期数据，周日为1，周一为2以此类推
//  rtc.time(t);//向DS1302设置时间数据
//}

void readtime() {
  Time tim = rtc.time(); //从DS1302获取时间数据
  shi_L = (tim.hr) % 10;
  shi_H = (tim.hr) / 10;
  fen_L = (tim.min) % 10;
  fen_H = (tim.min) / 10;
  miao_L = (tim.sec) % 10;
  miao_H = (tim.sec) / 10;
}

void display()
{ //先发送8位位码，后发送8位段码
  //8位数码管需要发送8次
  //保存段值/

  i%=8;
  duanZhi[7] = smgduan[miao_L];
  duanZhi[6] = smgduan[miao_H];
  duanZhi[3] = smgduan[fen_H];
  duanZhi[4] = smgduan[fen_L];
  duanZhi[1] = smgduan[shi_L];
  duanZhi[0] = smgduan[shi_H];
    digitalWrite(ST, LOW);    
    shiftOut(DS, SH, MSBFIRST, weiMa[i]);////送位码
    shiftOut(DS, SH, MSBFIRST, duanZhi[i]);//送段码
      i++;
    delayMicroseconds(2);
    digitalWrite(ST, HIGH);//ST_CP
    delay(2); 
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
  readtime();
}

void loop() {
  unsigned long currentMillis = millis();  
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    Time tim = rtc.time(); //从DS1302获取时间数据
//    miao = tim.sec;
//     shi = tim.hr;
//     fen = tim.min;
    miao_L = (tim.sec) % 10;
    miao_H = (tim.sec) / 10;
     
  fen_L = (tim.min) % 10;
  fen_H = (tim.min) / 10;
       
  shi_L = (tim.hr) % 10;
  shi_H = (tim.hr) / 10;
  }
display();
}
