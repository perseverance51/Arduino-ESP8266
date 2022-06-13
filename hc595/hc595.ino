#define ST  8   //ST CP of 74HC595移位寄存器时钟引脚
#define HS  12    //SH CP of 74HC595存储寄存器时钟输入引脚
#define DS  11 //DS of 74HC595串行数据输入引脚
#define interruptPin 2    //定义中引脚2或者3号引脚都可以
#define led 6   //接HC595的MR或者OE引脚,低电平时，清空移位寄存器中已有的bit数据
volatile byte State = LOW;//初始状态

void setup ()
{
  pinMode(led, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), use, FALLING);//开中断，下降沿触发
  pinMode(ST, OUTPUT); //锁存引脚设置
  pinMode(HS, OUTPUT); //时钟引脚设置
  pinMode(DS, OUTPUT); //数据引脚设置
}

void loop()
{
digitalWrite(led, State);
    HC595();
}
void use() {
  State = !State;
}

void HC595() {
  digitalWrite(ST, LOW); //将ST_CP口上面加低电平让芯片准备好接收数据
  shiftOut(DS, HS, MSBFIRST, 0x07); // 把0x07二进制0000 0111的值送给595
  //用MSBFIRST参数与LSBFIRST可以改变点亮的顺序
  digitalWrite(ST, HIGH); //将ST_CP这个针脚恢复到高电平
  delay(300);
    digitalWrite(ST, LOW); //将ST_CP口上面加低电平让芯片准备好接收数据
  shiftOut(DS, HS, MSBFIRST, 0x0e); // 把0x0e二进制0000 1110的值送给595
  //用MSBFIRST参数与LSBFIRST可以改变点亮的顺序
  digitalWrite(ST, HIGH); //将ST_CP这个针脚恢复到高电平
  delay(300);
    digitalWrite(ST, LOW); //将ST_CP口上面加低电平让芯片准备好接收数据
  shiftOut(DS, HS, MSBFIRST, 0x1c); // 把0x1c二进制0001 1100的值送给595
  //用MSBFIRST参数与LSBFIRST可以改变点亮的顺序
  digitalWrite(ST, HIGH); //将ST_CP这个针脚恢复到高电平
  delay(300);
    digitalWrite(ST, LOW); //将ST_CP口上面加低电平让芯片准备好接收数据
  shiftOut(DS, HS, MSBFIRST, 0x38); // 把0x38二进制0011 1000的值送给595
  //用MSBFIRST参数与LSBFIRST可以改变点亮的顺序
  digitalWrite(ST, HIGH); //将ST_CP这个针脚恢复到高电平
  delay(300);
  digitalWrite(ST, LOW); //将ST_CP口上面加低电平让芯片准备好接收数据
  shiftOut(DS, HS, MSBFIRST, 0x70); // 把0x70二进制0111 0000的值送给595
  //用MSBFIRST参数与LSBFIRST可以改变点亮的顺序
  digitalWrite(ST, HIGH); //将ST_CP这个针脚恢复到高电平
  delay(300);
   digitalWrite(ST, LOW); //将ST_CP口上面加低电平让芯片准备好接收数据
  shiftOut(DS, HS, MSBFIRST, 0xe0); // 把0xe0二进制1110 0000的值送给595
  //用MSBFIRST参数与LSBFIRST可以改变点亮的顺序
  digitalWrite(ST, HIGH); //将ST_CP这个针脚恢复到高电平
  delay(300);
  
}
