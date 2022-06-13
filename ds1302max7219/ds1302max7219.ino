
#include <stdio.h>
#include <DS1302.h>
#include <LedControl.h>//点击这里会自动打开管理库页面: http://librarymanager/All#LedControl

#define interruptPin 2
volatile byte State = LOW;//中断中的变量使用volatile 修饰词

// inputs: DIN pin, CLK pin, LOAD pin. number of chips
LedControl mydisplay = LedControl(11, 9, 10, 1);
DS1302 rtc(8, 7, 6); //对应DS1302的RST,DAT,CLK


unsigned long previousMillis = 0;
const long interval = 1000; //设置延时时间间隔
uint8_t buf[15];//存放时间数据

void initRTCTime(void)//初始化RTC时钟
{
  rtc.writeProtect(false); //关闭写保护
  rtc.halt(false); //清除时钟停止标志

  Time t(2021, 12, 11, 10, 34, 50, 6); //创建时间对象 最后参数位，为星期数据，周日为1，周一为2，周二为3，周四为5以此类推.
  rtc.time(t);//向DS1302设置时间数据
}

/******获取时间函数****/
void printTime(){
  //strcpy(buf, "");
   Time tim = rtc.time(); //从DS1302获取时间数据
  memset(buf, 0, sizeof(buf));
  snprintf(buf, sizeof(buf), "%02d%02d%02d%02d%02d%02d",
           tim.hr, tim.min, tim.sec, tim.yr, tim.mon, tim.date);
    if(State){     
  mydisplay.clearDisplay(0);
  mydisplay.setChar(0, 0, buf[8], false);//年-高位
  mydisplay.setChar(0, 1, buf[9], false);//年-低位
  mydisplay.setChar(0, 2, '-', false);//-
  mydisplay.setChar(0, 3, buf[10], false); //月-高位
  mydisplay.setChar(0, 4, buf[11], false);//月-低位
  mydisplay.setChar(0, 5, '-', false);//-
  mydisplay.setChar(0, 6, buf[12], false);//日-高位
  mydisplay.setChar(0, 7, buf[13], false); //日-低位   
      }else{
  mydisplay.clearDisplay(0);
  mydisplay.setChar(0, 0, buf[0], false);//时-高位
  mydisplay.setChar(0, 1, buf[1], false);//时-低位
  mydisplay.setChar(0, 2, '-', false);//-
  mydisplay.setChar(0, 3, buf[2], false); //分-高位
  mydisplay.setChar(0, 4, buf[3], false);//分-低位
  mydisplay.setChar(0, 5, '-', false);//-
  mydisplay.setChar(0, 6, buf[4], false);//秒-高位
  mydisplay.setChar(0, 7, buf[5], false); //秒-低位      
      }
}


void setup() {
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), use, FALLING);//开中断，下降沿触发
/********** 
   //DS1302新模块第一次上电需要设置一次当前时间进去，
  //下载完成后需屏蔽此函数再次下载，否则每次上电都会初始化时间数据
  // initRTCTime();
**********/   
  rtc.writeProtect(true);//ds1302写保护开启
  for (int i = 0; i < DS1302::kRamSize; ++i) {
    rtc.writeRam(i, 0x00);
  } 
  mydisplay.shutdown(0, false);  // 初始化时设置点阵（设备）的关机（节能）模式
  mydisplay.setIntensity(0, 15); //设置亮度值 0-15

}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;  
    printTime();    
  }
}
/**中断服务函数**/
void use() {
  State = !State;
}
