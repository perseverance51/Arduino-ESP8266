#include <ESP_EEPROM.h>////点击这里会自动打开管理库页面: http://librarymanager/All#ESP_EEPROM
//定义一个读和写的通用方法
#define EEPROM_write(address,p) { int i = 0;byte *pp = (byte*)&(p);for(;i<sizeof(p);i++) EEPROM.write(address+i,pp[i]); EEPROM.commit();}
#define EEPROM_read(address,p) { int i = 0;byte *pp = (byte*)&(p);for(;i<sizeof(p);i++) pp[i]=EEPROM.read(address+i);}
//注意事项 写的结尾一定要有EEPROM.end();或EEPROM.commit();目的就是提交保存操作否则不保存成功

//测试2个结构体不同的存储地地址
struct datex
{ //结构体1
  int a;
  float b;
  char c[64];
} date1; //变量名

int ron = sizeof(date1);
struct date_
{ //结构体2
  char y[128];
} date2; //变量名
int ron2 = sizeof(date2);
/*-------写操作---------*/
void eeprom_write() {
  Serial.println(sizeof(date1)); 
  date1.a = 9527;
  date1.b = 3.14;
   strcpy(date1.c, String("{\"perseverance\":\"courge\",\"green\":\"9527\"}").c_str());  // 标准json字符串；赋值方法strcpy()
 //strcpy(date1.c, "{\"perseverance\":\"courge\",\"green\":\"9527\"}");这样写，验证了也可以 
  EEPROM.begin(204);//申请内存空间
  EEPROM_write(0, date1);  //写入到0地址的结构体变量
// EEPROM.put(0, date1);
// EEPROM.commitReset();
 // EEPROM.commit();  //上方法中提交  
 delay(10);
  //-------结构体1写结束-------------
  sprintf(date2.y, "{\"evergreen\":\"%s\",\"mode\":%d,\"automode\":%d}","12456",1,0); //赋值方法 标准json格式
  EEPROM_write(76, date2);//写地址128+1 结构体2
 // EEPROM.put(76, date2);
  Serial.println("write ok!");
}
/*-------读操作---------*/
void eeprom_read() { 
 memset(&date1, 0, sizeof(date1));//清空结构体成员数据
 memset(&date2, 0, sizeof(date2));
//  date1.a=0;
//  date1.b =0;
// date1.c[64] ={0};
// date2.y[128]={0};
  //-----------结构体1读------------
    //-----读取EEPROM数据时不能再使用 EEPROM.begin();该函数是用来申请空间的，否则，就破坏了要读取区域的数据！！！
  EEPROM_read(0, date1);  //从地址0读取数据
//  EEPROM.get(0, date1);
//  EEPROM.commitReset();
  Serial.print("a:");
  Serial.println(date1.a);//打印数据1
  Serial.print("b:");
  Serial.println(date1.b);
  Serial.print("c:");
  Serial.println(date1.c);//打印数据3
 delay(10); 
  //------------结构体2读----------------
  EEPROM_read(76, date2);//读数据
//  EEPROM.get(76, date2);//读数据
  Serial.print("y:");
  Serial.println(date2.y);//打印数据

  Serial.println("read eeprom over!");
}

void setup() {
  Serial.begin(9600);  
 eeprom_write();//读和写
}

void loop() {
   Serial.print("date1:\t"); Serial.println(sizeof(date1));
   Serial.print("date2:\t"); Serial.println(sizeof(date2));
  delay(8000);
  eeprom_read();
}
