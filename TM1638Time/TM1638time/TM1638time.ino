//#include <EEPROM.h>

#include "TM1638self.h"
#include <Wire.h>
#include <DS3231.h>

//时钟管脚定义
#define SCL A5    //      I2C-SCL
#define SDA A4    //       I2C-SDA   
#define INT_SQW 2   // D5    // 中断及方波1HZ
#define BUILTIN_LED 13
//TM1638管脚定义
#define STB 11  // TM1638 STB 
#define CLK 12  //   TM638 DIO
#define DIO 13  //   TM1638 CLK

/// 系统变量/
const long interval = 1000;//设置延时时间间隔
unsigned long previousMillis = 0;

unsigned char showtime[10]={0,0,0,0,0,0,0,0,0,0};   //the time  show on led  now.最后两位是温度
int SecondInterruptMode=0;//0显示时间，1显示配网提示。

DS3231 RTC;      //创建DT3231时钟
int secondFlag=0;  //秒闪标志用于控制闪灯是亮还是灭的标志
unsigned char keynum=0; //读到的键盘值

TM1638  TM1638(CLK,DIO,STB);   //建立TM638对象

enum Mode {     //杖举型，用于判定按键一处于什么模式，使+-键进行对应作用
  VOLUME=1,
  MUSIC=2
  } ; 
Mode NowMode=VOLUME;

//resetup等等按键的中断处理程序
void keyProcessing(unsigned char num){  
  switch (num){   //按键值为1-8时
    case 1:    
    //按键处理程序。。。。   
      break;
    case 2:    
    //按键处理程序。。。。  
       break;
    case 3:
      //按键处理程序。。。。
        break;
    case 8:
    //按键处理程序。。。。  
      break; 
  }

}

///时间函数///
void GetTime(){    //给显示变量赋值   
      showtime[0]=RTClib::now().month()/10;     //月 
      showtime[1]=RTClib::now().month()%10;     
      showtime[2]=RTClib::now().day()/10;      //日
      showtime[3]=RTClib::now().day()%10;
      showtime[4]=RTClib::now().hour()/10;      //时
      showtime[5]=RTClib::now().hour()%10;
      showtime[6]=RTClib::now().minute()/10;    //分
      showtime[7]=RTClib::now().minute()%10;
      showtime[8]=int(RTC.getTemperature())/10;  //温度
      showtime[9]=int(RTC.getTemperature())%10; 
}

void second_interrupt(int mode){ //时钟每秒中断
  if (mode==0){        //模式0 用于秒闪灯工作以及显示时间。模式1用于显示其它
     //秒灯闪
    switch (secondFlag){
      case 0:  //秒灯灭
        TM1638.SecondBlink(1,showtime[5]); //只重显示小时个位
        secondFlag=1;
        break;
      case 1:  //秒灯亮
        TM1638.SecondBlink(0,showtime[5]);
        secondFlag=0;
        break;
    }
    
    //显示时间
    if ( showtime[7]!=RTClib::now().minute()%10 ) {   //只要比较，分不同就重新显示

      GetTime();
      TM1638.Disp8(showtime);    
    }  
  }
}

//主启动程序和主调用程序 

void setup()
{
  Serial.begin(115200);  
     pinMode(BUILTIN_LED,OUTPUT);
  // 显示器初始化//
   TM1638.SelfTest();

//   设置闹钟中断输出设置秒闪功能///
//   pinMode(INT_SQW,INPUT_PULLUP); 
//   interrupts();
//   attachInterrupt(digitalPinToInterrupt(2),alarm_interrupt,FALLING);   //支持的触发方式CHANGE RISING FALLING   
}

void loop()
{
   unsigned long currentMillis = millis();      
  if (currentMillis - previousMillis >= interval) {//一秒钟更新一次时间
     previousMillis = currentMillis;
    second_interrupt(keynum);
    digitalWrite(BUILTIN_LED, !digitalRead(BUILTIN_LED));//状态翻转
  }
   keynum=TM1638.ReadKey();
    if (keynum==9) keynum=0;
    while(keynum==TM1638.ReadKey());  //等待按键释放
    if ((keynum>0)&&(keynum<9)){
      keyProcessing(keynum);    
    }
}
