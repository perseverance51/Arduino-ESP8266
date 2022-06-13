int latchPin=8; //ST CP of 74HC595
int clockPin=12;//SH CP of 74HC595
int dataPin=11; //DS of 74HC595
unsigned char smgduan[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};//显示0~9的值共阴
//unsigned char duanMa[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80}; //选择1-8哪个数码管段
void setup() {
      pinMode(latchPin, OUTPUT);
      pinMode(clockPin, OUTPUT);
      pinMode(dataPin, OUTPUT); 
}
void loop()
{
   for(int i=0; i<99; i++)
   {
    unsigned char gewei = i%10;
    unsigned char shiwei = i/10;  
      digitalWrite(latchPin,LOW); //低电位表示启动
      shiftOut(dataPin,clockPin,MSBFIRST,smgduan[shiwei]);
      shiftOut(dataPin,clockPin,MSBFIRST,smgduan[gewei]);
      digitalWrite(latchPin,HIGH); //高电位表示停止
      delayMicroseconds(2);
      digitalWrite(latchPin, HIGH);//ST_CP
      delay(500);
   }

}
