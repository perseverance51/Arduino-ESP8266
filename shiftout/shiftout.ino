int latchPin=8; //ST CP of 74HC595
int clockPin=12;//SH CP of 74HC595
int dataPin=11; //DS of 74HC595
char LED_pin=0x00;
void setup() {
      pinMode(latchPin, OUTPUT);
      pinMode(clockPin, OUTPUT);
      pinMode(dataPin, OUTPUT); 
}
void loop()
{
   for(int i=0; i<8; i++)
   {
      LED_pin=~(0x01<<i);
      digitalWrite(latchPin,LOW); //低电位表示启动
      shiftOut(dataPin,clockPin,MSBFIRST,~LED_pin);
      digitalWrite(latchPin,HIGH); //高电位表示停止
      delay(500);
   }
     LED_pin=0x01;
}
