#include <Wire.h>


void Writebyte(byte DeviceAddress, byte DataAddress,  unsigned char Data)
{
  int rData = Data;
  Wire.beginTransmission(DeviceAddress);
  Wire.write(DataAddress);
  Wire.write(rData);
  Wire.endTransmission();
  delay(10);
}

char Readbyte(int DeviceAddress, unsigned char DataAddress)
{
  byte rdata = 0xFF;
  Wire.beginTransmission(DeviceAddress);
  Wire.write(DataAddress);
  Wire.endTransmission();
  Wire.requestFrom(DeviceAddress, 1);
  delay(10);
  if (Wire.available())
  {
    rdata = Wire.read();
  }
  delay(10);
  return rdata;
}

void setup()
{
  Wire.begin();
  Serial.begin(115200);
  
      char str[]="perseverance51";
  unsigned char shu =sizeof(str)/sizeof(char);
      /* 向AT24C02写入数据 */
  for(byte i=0;i<shu;i++){
     Writebyte(0x50,i,str[i]);
    delay(5);//这个延时必不可少，否则写入失败！
    }
}

void loop()
{
 char*str0 ="";
    char*str2="";
     Serial.print("Data read in AT24C02:"); 
    /* 读取AT24C02里的数据 */
    for(byte i=0;i<12;i++)
    {
    str0 = Readbyte(0x50, i);
      delay(5);
      str0++;
    }  
         delay(500);  
    Serial.print("AT24C02读取的第一段数据:");    
    Serial.println(String(str0));
    Serial.println("数据读取完成!");
    str0 ="";//读取完成，做数据清空。
   // Serial.flush();
    delay(3000);  
    
  /* 读取AT24C02里的数据 */
    for(byte i=12;i<15;i++)
    {
       Readbyte(0x50, i);
      delay(5);
      str2++;
    }    
      delay(500);  
    Serial.print("AT24C02读取的第二段数据:"); 
    Serial.println(String(str0));
    Serial.println("数据读取完成!");
    str2 ="";//读取完成，做数据清空。
          delay(5000);  
}
