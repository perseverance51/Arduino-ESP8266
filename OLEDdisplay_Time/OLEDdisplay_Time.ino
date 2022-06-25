/*
 * 国内NTP服务器地址
 * ntp1.aliyun.com - ntp7.aliyun.com可选
 * cn.ntp.org.cn
 * ESP8266   I2C PINS: SDA GPIO4/ SCL GPIO5
 *  Ctrl + Shift + I 
 */
 #ifdef ESP8266
#include <ESP8266WiFi.h>//esp8266开发板自带库
#define SDA  D2 //对应nodemcu接D1，，wemosD1mini的D2
#define SCL  D1 //对应nodemcu接D2，，wemosD1mini的D5
#elif  ESP32
#include <WiFi.h> // esp32开发板自带库
#define SDA  (22) //ESP32 SDA
#define SCL  (21) //ESP32 SCL
#endif
#include <NTPClient.h>//点击这里会自动打开管理库页面: http://librarymanager/All#NTPClient
//#include "ssd1306.h"//点击这里会自动打开管理库页面: http://librarymanager/All#SSD1306
#include "SSD1306Wire.h"//0.96寸IIC OLED
//#include "OLEDDisplayUi.h"
#include <WiFiUdp.h>
/*填写wifi信息*/
const char *ssid= "MERCURY_D268G";
const char *password = "pba5ayzk";

#define I2C_DISPLAY_ADDRESS  0x3c     //SSD1306 OLED i2c地址默认
SSD1306Wire display(0x3c, SDA, SCL);   // ADDRESS, SDA, SCL  -  SDA and SCL usually populate automatically based on your board's pins_arduino.h e.g. https://github.com/esp8266/Arduino/blob/master/variants/nodemcu/pins_arduino.h
// SH1106Wire display(0x3c, SDA, SCL);//1.3寸屏幕使用这个

WiFiUDP ntpUDP;
// NTPClient(UDP &udp, const char *poolServerName, int timeOffset);
//NTPClient timeClient(ntpUDP);//默认NTP：time.nist.gov，
//NTPClient timeClient(ntpUDP,60*60*8);//添加偏移时间
NTPClient timeClient(ntpUDP,"ntp3.aliyun.com",60*60*8);//timeOffset：0,国内属于东8区,以秒为偏移单位
// NTPClient(UDP &udp, const char *poolServerName, int timeOffset, int updateInterval);//updateInterval:自定义(在测试过程中设置了不见效果)

#define screenW 128
#define screenH  64
unsigned char clockCenterX = screenW / 2;
//int clockCenterY = ((screenH - 16) / 2) + 16; // 0.96双色屏幕，避开黄色区域 16 px height
unsigned char clockCenterY = screenH  / 2;
unsigned char clockRadius = 32;
//实用功能的数字时钟显示:打印前导0
String twoDigits(int digits) {
  if (digits < 10) {
    String i = '0' + String(digits);
    return i;
  }
  else {
    return String(digits);
  }
}
void analogClockFrame();
void setup(){
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED ) {//没有入网情况下在这里运行
    delay ( 500 );
    Serial.print ( "." );
  }
  timeClient.begin();
  display.init();
  display.clear();
    display.flipScreenVertically();
  display.display();
   display.setContrast(240); //屏幕亮度

  
}

void loop() {  
//  timeClient.update();
//  Serial.println(timeClient.getFormattedTime());
//  char hours = timeClient.getHours();
//  char minu =  timeClient.getMinutes();
//  char sec =  timeClient.getSeconds();
//  Serial.printf("hour:%d minu:%d sec:%d\n", hours,minu,sec);
  analogClockFrame();
  delay(1000);
  display.clear();
}

void analogClockFrame() {
  timeClient.update();
  Serial.println(timeClient.getFormattedTime());
// display.drawCircle(clockCenterX, clockCenterY , clockRadius);//钟盘外圈
  display.drawCircle(clockCenterX, clockCenterY, 2);//圆心
  //小时刻度
  for ( int z = 0; z < 360; z = z + 30 ) {
    //Begin at 0° and stop at 360°
    float angle = z ;
    angle = ( angle / 57.29577951 ) ; //Convert degrees to radians
    int x2 = ( clockCenterX + ( sin(angle) * clockRadius ) );
    int y2 = ( clockCenterY - ( cos(angle) * clockRadius ) );
    int x3 = ( clockCenterX + ( sin(angle) * ( clockRadius - ( clockRadius / 8 ) ) ) );
    int y3 = ( clockCenterY - ( cos(angle) * ( clockRadius - ( clockRadius / 8 ) ) ) );
    display.drawLine( x2  , y2  , x3  , y3);
     display.display();
  }

  // 秒指针
  float angle = timeClient.getSeconds() * 6 ;
  angle = ( angle / 57.29577951 ) ; //Convert degrees to radians
  int x3 = ( clockCenterX + ( sin(angle) * ( clockRadius - ( clockRadius / 5 ) ) ) );
  int y3 = ( clockCenterY - ( cos(angle) * ( clockRadius - ( clockRadius / 5 ) ) ) );
  display.drawLine( clockCenterX , clockCenterY, x3, y3);
  //
  // 分指针
  angle = timeClient.getMinutes() * 6 ;
  angle = ( angle / 57.29577951 ) ; //Convert degrees to radians
  x3 = ( clockCenterX + ( sin(angle) * ( clockRadius - ( clockRadius / 4 ) ) ) );
  y3 = ( clockCenterY - ( cos(angle) * ( clockRadius - ( clockRadius / 4 ) ) ) );
  display.drawLine( clockCenterX , clockCenterY, x3, y3);
  //
  // 小时指针
  angle = timeClient.getHours() * 30 + int( ( timeClient.getMinutes() / 12 ) * 6 )   ;
  angle = ( angle / 57.29577951 ) ; //Convert degrees to radians
  x3 = ( clockCenterX + ( sin(angle) * ( clockRadius - ( clockRadius / 2 ) ) ) );
  y3 = ( clockCenterY - ( cos(angle) * ( clockRadius - ( clockRadius / 2 ) ) ) );
  display.drawLine( clockCenterX, clockCenterY, x3 , y3);
   display.display();
}
