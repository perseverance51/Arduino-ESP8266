int latchPin=8; //ST CP of 74HC595
int clockPin=12;//SH CP of 74HC595
int dataPin=11; //DS of 74HC595
unsigned char smgduan[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90}; //共阳段码表
//{0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};//显示0~9的值共阴
unsigned char WeiXian[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80}; //选择1-8哪位数码管段
static unsigned char i =0,k=0;
unsigned char duanZhi[8] = {0, 0, 0, 0, 0, 0, 0, 0};
const long interval = 1000;//设置延时时间间隔
unsigned long previousMillis = 0;
unsigned char gewei,shiwei,baiwei,qian;
void display();
void setup() {
      pinMode(latchPin, OUTPUT);
      pinMode(clockPin, OUTPUT);
      pinMode(dataPin, OUTPUT); 
}
void loop()
{   
    i %=10000;
   unsigned long currentMillis = millis();     
    display();

   if (currentMillis - previousMillis >= interval) {
    i++;
    previousMillis = currentMillis;
   } 

}
void display()
{ //先发送8位位码，后发送8位段码
  //8位数码管需要发送8次
  //保存段值/
      k%=8;
     gewei = ((i%1000)%100)%10;
     shiwei = ((i%1000)%100)/10;
     baiwei = (i%1000)/100; 
     qian = i/1000; 
    
  duanZhi[3] = smgduan[gewei];
  duanZhi[2] = smgduan[shiwei];
  duanZhi[1] = smgduan[baiwei];
  duanZhi[0] = smgduan[qian];
  
      digitalWrite(latchPin,LOW); //低电位表示启动      
      shiftOut(dataPin,clockPin,MSBFIRST,WeiXian[k]);
      shiftOut(dataPin,clockPin,MSBFIRST,duanZhi[k]);      
       k++;
      digitalWrite(latchPin,HIGH); //高电位表示停止
      delayMicroseconds(2);
      digitalWrite(latchPin, HIGH);//ST_CP
}
