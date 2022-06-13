/*
   Wire库I2C基本读写程序
   AT24C32读写操作
   /* 描述：AT24C32写入一个字节函数
   参数：date_wr:    要写入的值
        WriteAddr:  要写入的地址
   返回值：通信成功：0
          通信失败：1->数据溢出   2->发送addtess时从机接受到NACK
                  3->发送数据时接受到NACK  4->其他错误
*/


#include <Wire.h>                           // 使用Wire库，需要包含头文件

#define button 7
#define I2C_ADDR 0x50   // AT24Cxx的IIC器件地址0x50,这个地址可以通过I2C扫描程序搜索到。

word wordAddress = 0x1000;
char*buf = "perseverance9527";
 char *str1 = "";
bool buttonState = true, flag = true;

uint8_t AT24Cxx_write(char data_wr, word WriteAddr)
{
  /* 1. 开始一次传输，设置I2C器件地址 */
  Wire.beginTransmission(I2C_ADDR);
  /* 2. 需要先写入高位的地址位 */
  Wire.write(highByte(WriteAddr));
  /* 3. 写入低位的地址位 */
  Wire.write(lowByte(WriteAddr));
  /* 4. 需要写入的值 */
  Wire.write(byte(data_wr));
  /* 5. 完成一次I2C通信，默认发送一个停止位 */
  return Wire.endTransmission();
}
uint8_t AT24Cxx_read(char *data_wr, word ReadAddr)
{
  uint8_t ret = 0;
  /* 1. 开始一次传输，设置I2C器件地址 */
  Wire.beginTransmission(I2C_ADDR);
  /* 2. 需要先写入高位的地址位 */
  Wire.write(highByte(ReadAddr));
  /* 3. 写入低位的地址位 */
  Wire.write(lowByte(ReadAddr));
  /* 4. 完成一次I2C通信，发送一个开始位(即重发码) */
  ret = Wire.endTransmission();
  /* 5. 开始一次读取，设置I2C器件地址，读取AT24Cxx一个字节 */
  Wire.requestFrom(I2C_ADDR, 1);
  /* 5. 读出AT24CXX指定地址的值，并将值转化为char后赋值给指针 */
  *data_wr = char(Wire.read()); // receive a byte as character
  delay(10);
  return ret;
}
void writeData(word addr, byte data)
{
  Wire.beginTransmission(I2C_ADDR);
  Wire.write((int)(addr >> 8));
  Wire.write((int)(addr & 0xFF));
  Wire.write(data);
  Wire.endTransmission();
  delay(10);
}

byte readData(word addr)
{
  byte result;
  Wire.beginTransmission(I2C_ADDR);
  // set the pointer position
  Wire.write((int)(addr >> 8));
  Wire.write((int)(addr & 0xFF));
  Wire.endTransmission();
  Wire.requestFrom(I2C_ADDR, 1); // get the byte of data
  if (Wire.available() > 0)

    result = Wire.read();
  return result;
}
void WriteStr(char *str0, word WriteAddr) {
  char *pStr = str0;
  Serial.println(strlen(str0));
  for (int i = 0; i < strlen(str0); i++) {
    AT24Cxx_write(*pStr++, WriteAddr++);
    delay(20);
  }
}
void ReadStr(char *data_wr, word ReadAddr, uint8_t len) {
  for (int i = 0; i < len; i++) {
    AT24Cxx_read(data_wr++, ReadAddr++);
    delay(20);
  }
}

void setup() {
  Wire.begin();
  /* 初始化串口波特率为9600 */
  Serial.begin(9600);
  digitalWrite(LED_BUILTIN, LOW);//板子led，作为观察数据发射状态
  pinMode(button, INPUT);
  digitalWrite(button, HIGH);

  // WriteStr(buf,0x0010);
  delay(10);
}
void loop() {
  buttonState = digitalRead(button);

    if (buttonState == LOW && flag == true) {
      flag = false;
      Serial.println("Trigger!!!");

      ReadStr(str1, 0x0010, strlen(buf));
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      delay(1500);
      Serial.println(str1);
      Serial.flush();
      str1 = "";
      delay(2000);
      flag = true;
  }
}
