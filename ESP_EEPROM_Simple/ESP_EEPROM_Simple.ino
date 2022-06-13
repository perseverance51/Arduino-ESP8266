// ESP8266的ESP_EEPROM库使用示例
//
//通常在ESP8266上写入'仿真' EEPROM需要擦除用于保存的闪存页
//对EEPROM数据进行重写。
//删除需要大量的时间(10s of ms)，在此期间
//中断必须被阻塞。
//在某些情况下，这会干扰草图操作(例如，它会产生一个明显的
// ESP8266 PWM依赖于中断时，任何PWM控制的灯都会停电/闪光)
//
// ESP_EEPROM库将每个新版本的EEPROM数据写入一个新区域，直到flash结束
//已满，因此在必要时避免擦除flash
//
//当EEPROM中只有几个东西需要保存时，它是最好的
//(即保存的信息的总大小比可用的flash要小得多)

struct MyObject {
  char name[15];//15
  float field1;//4
  int field2;//2
  byte Height; //1
};

struct MyObject2 {
  char name2[15];//15
  float field1a;//4
  int field2b;//2
  byte Height1; //1
};
#include <ESP_EEPROM.h>

int eepromVar1 = 0;
long eepromVar2 = 1234;

void setup() {
  // Remember to set your serial monitor to 74880 baud
  // This odd speed will show ESP8266 boot diagnostics too
  Serial.begin(9600);
  Serial.println();

  // All the library functions are accessed via the EEPROM object created when
  // you include the library header ESP_EEPROM.h

  // 库需要知道申请的EEPROM变量空间需要多大
  // 最小大小为16

  // The begin() call is required to initialise the EEPROM library
  EEPROM.begin(1024);

  // put some data into eeprom
  EEPROM.put(0, eepromVar1);  // int - so 4 bytes (next address is '4')
  EEPROM.put(4, eepromVar2);  // long - so 8 bytes (next address would be '12')
MyObject customVar = {
     "perseverance52",
    3.14f,
    9527,
    64,  
  };
  // write the data to EEPROM
  boolean ok1 = EEPROM.commit();
  Serial.println((ok1) ? "First commit OK" : "Commit failed");

  // The eeprom data gets changed
  eepromVar1 = 1;   // Change some data

  // set the EEPROM data ready for writing
  EEPROM.put(0, eepromVar1);

  // commit (write) the data to EEPROM - only actually writes if there has been a change
  boolean ok2 = EEPROM.commit();
  Serial.println((ok2) ? "Second commit OK" : "Commit failed");

  // How to read stuff back into variables
  // variables should be same size as originally written
  int aNewVar1;
  long aNewVar2;
  EEPROM.get(0, aNewVar1);
  EEPROM.get(4, aNewVar2);
  Serial.print("Read back a variable 1 from EEPROM: ");
  Serial.println(aNewVar1);
  Serial.print("Read back a variable 2 from EEPROM: ");
  Serial.println(aNewVar2);
   EEPROM.put(8, customVar);
 bool ok3 = EEPROM.commit();
  Serial.println((ok3) ? "Struct commit OK" : "Struct Commit failed");
}


void loop() {
  delay(10000);
  MyObject2 customVar2;
     EEPROM.get(8, customVar2);
     Serial.println( "Read custom object from EEPROM: " );
   Serial.println( customVar2.name2);//打印获取的数据
 Serial.println( customVar2.field1a );
  Serial.println( customVar2.field2b );
  Serial.println( customVar2.Height1 );

  

}
