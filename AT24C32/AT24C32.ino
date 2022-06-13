/*
 * Wire库I2C基本读写程序
 * AT24C32读写操作
 * /* 描述：AT24C32写入一个字节函数
 * 参数：date_wr:    要写入的值
 *      WriteAddr:  要写入的地址
 * 返回值：通信成功：0 
 *        通信失败：1->数据溢出   2->发送addtess时从机接受到NACK 
 *                3->发送数据时接受到NACK  4->其他错误
 */


#include <Wire.h>                           // 使用Wire库，需要包含头文件
           
#define button 7
#define I2C_ADDR 0x50   // AT24Cxx的IIC器件地址0x50,这个地址可以通过I2C扫描程序搜索到。
 
word wordAddress = 0x1000; 
 
bool buttonState = true; 

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
    *data_wr= char(Wire.read()); // receive a byte as character
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
  Wire.requestFrom(I2C_ADDR,1); // get the byte of data
  if (Wire.available() > 0) 

  result = Wire.read();
  return result;
}
void setup(){
  Wire.begin(); 
  /* 初始化串口波特率为115200 */
  Serial.begin(115200); 
  digitalWrite(LED_BUILTIN, LOW);//板子led，作为观察数据发射状态
  pinMode(button,INPUT);
  digitalWrite(button,HIGH);
  delay(10); 
} 
void loop(){
 buttonState = digitalRead(button);
    if(buttonState == LOW){
      delay(250);
          if(buttonState == LOW){
            char str0 = 'R';
//      uint8_t Write = AT24Cxx_write(str0, wordAddress);
    writeData(wordAddress, byte(str0));
       Serial.println("Trigger!!!");
//       Serial.println(Write);
       digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
       delay(2000);
       byte buf= readData(wordAddress);
        Serial.println(buf);
       Serial.println(char(buf)); 
  //    char*str2;
 //    uint8_t Read = AT24Cxx_read(str2,wordAddress);
 //     Serial.println(*str2);Serial.println(Read);
    }    
    }
}
