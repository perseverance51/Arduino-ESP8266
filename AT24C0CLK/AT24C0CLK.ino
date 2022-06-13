/*
 * Wire库I2C基本读写程序
 * AT24C02读写操作
 *
*/

#include <Wire.h>                           // 使用Wire库，需要包含头文件
#define I2C_ADDR   0x50             // AT24C02的IIC器件地址0x50,这个地址可以通过I2C扫描程序搜索到。
#define button 7

bool buttonState = true; 
/* 描述：AT24C02写入一个字节函数
 * 参数：date_wr:    要写入的值
 *      WriteAddr:  要写入的地址
 * 返回值：通信成功：0 
 *        通信失败：1->数据溢出   2->发送addtess时从机接受到NACK 
 *                3->发送数据时接受到NACK  4->其他错误
 */
uint8_t at24c02_write(char data_wr, uint8_t WriteAddr)
{
  /* 1. 开始一次传输，设置I2C器件地址 */
  Wire.beginTransmission(I2C_ADDR);
  /* 2. 需要写入的位置 */
  Wire.write(WriteAddr);
  /* 3. 需要写入的值 */
  Wire.write(data_wr);
  /* 4. 完成一次I2C通信，默认发送一个停止位 */
  return Wire.endTransmission();
}
/* 描述：AT24C02读取一个字节函数
 * 参数：date_wr:    要读出值的存放指针
 *      WriteAddr:  要读出的地址
 * 返回值：通信成功：0  
 *        通信失败：1->数据溢出   2->发送address时从机接受到NACK 
 *                3->发送数据时接受到NACK  4->未接受到数据  5->其他
 */
uint8_t at24c02_read(char *data_wr, uint8_t ReadAddr)
{
  uint8_t t = 200;
  uint8_t ret = 0;
  /* 1. 开始一次传输，设置I2C器件地址 */
  Wire.beginTransmission(I2C_ADDR);

  /* 2. 需要读出的位置 */
  Wire.write(ReadAddr);

  /* 3. 完成一次I2C通信，发送一个开始位(即重发码) */
  ret = Wire.endTransmission(false);

  /* 4. 开始一次读取，设置I2C器件地址，读取AT24C02一个字节 */
  Wire.requestFrom(I2C_ADDR, 1);

  /* 5. 读出AT24C02返回的值，成功读取后写入缓存变量处，读取失败返回失败码 */
  while (!Wire.available()) 
  {
    t--;
    delay(1); 
    if(t == 0)
    {
      return 1;
    }
  }
  *data_wr= Wire.read(); // receive a byte as character
  return ret;
}

void setup() {
  /* 初始化串口波特率为115200 */
  Serial.begin(115200); 
  digitalWrite(LED_BUILTIN, LOW);//板子led，作为观察数据发射状态
  pinMode(button,INPUT);
  digitalWrite(button,HIGH);
  /* 初始化IIC接口，不写入地址则默认为主设备 */  
  Wire.begin();
//  Wire.begin(SDA, SCL);//esp8266指定SDA和SCL
//  Wire.begin(SDA, SCL);//Arduino UNO板使用不带参数的
//  delay(2000);
//  char str[]="perseverance51";
//  unsigned char shu =sizeof(str)/sizeof(char);
//      /* 向AT24C02写入数据 */
//  for(int i=0;i<shu;i++){
//    at24c02_write(str[i], i);
//    delay(5);//这个延时必不可少，否则写入失败！
//    }
}

void loop() {
 buttonState = digitalRead(button);
    if(buttonState == LOW){
      delay(250);
          if(buttonState == LOW){
            char str0 = 'H';
      uint8_t Write = at24c02_write(str0, 0xff);
       Serial.println("Trigger!!!");
       Serial.println(Write);
       digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
       delay(2000);
      char*str2;
     uint8_t Read = at24c02_read(str2,0xff);
      Serial.println(*str2);Serial.println(Read);
    }    
    }
}
