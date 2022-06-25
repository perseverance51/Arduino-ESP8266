
// Install https://github.com/PaulStoffregen/Time
//#include <TimeLib.h>
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
// Include the correct display library
// For a connection via I2C using Wire include
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // 0.96寸IIC 点击这里会自动打开管理库页面: http://librarymanager/All#SSD1306
// #include "SH1106Wire.h"//1.3寸
// For a connection via I2C using brzo_i2c (must be installed) include
// #include <brzo_i2c.h> // Only needed for Arduino 1.6.5 and earlier
// #include "SSD1306Brzo.h"
// #include "SH1106Brzo.h"
// For a connection via SPI include
// #include <SPI.h> // Only needed for Arduino 1.6.5 and earlier
// #include "SSD1306Spi.h"
// #include "SH1106SPi.h"

// Include the UI lib
#include "OLEDDisplayUi.h"

// Include custom images
#include "images.h"
#include <WiFiUdp.h>
/*填写wifi信息*/
const char *ssid= "MERCURY_D268G";
const char *password = "pba5ayzk";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP,"ntp3.aliyun.com",60*60*8);//timeOffset：0,国内属于东8区,以秒为偏移单位

SSD1306Wire display(0x3c, SDA, SCL);   // ADDRESS, SDA, SCL  -  SDA and SCL usually populate automatically based on your board's pins_arduino.h e.g. https://github.com/esp8266/Arduino/blob/master/variants/nodemcu/pins_arduino.h
// SH1106Wire display(0x3c, SDA, SCL);//1.3寸屏幕使用这个

OLEDDisplayUi ui ( &display );

int screenW = 128;
int screenH = 64;
int clockCenterX = screenW / 2;
int clockCenterY = ((screenH - 16) / 2) + 16; // 0.96双色屏幕，避开黄色区域 16 px height
int clockRadius = 23;

// utility function for digital clock display: prints leading 0
String twoDigits(int digits) {
  if (digits < 10) {
    String i = '0' + String(digits);
    return i;
  }
  else {
    return String(digits);
  }
}

//void clockOverlay(OLEDDisplay *display, OLEDDisplayUiState* state) {}

void analogClockFrame(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  timeClient.update();
  //  ui.disableIndicator();
  // Draw the clock face
  //  display->drawCircle(clockCenterX + x, clockCenterY + y, clockRadius);
  display->drawCircle(clockCenterX + x, clockCenterY + y, 2);
  //
  //hour ticks
  for ( int z = 0; z < 360; z = z + 30 ) {
    //Begin at 0° and stop at 360°
    float angle = z ;
    angle = ( angle / 57.29577951 ) ; //Convert degrees to radians
    int x2 = ( clockCenterX + ( sin(angle) * clockRadius ) );
    int y2 = ( clockCenterY - ( cos(angle) * clockRadius ) );
    int x3 = ( clockCenterX + ( sin(angle) * ( clockRadius - ( clockRadius / 8 ) ) ) );
    int y3 = ( clockCenterY - ( cos(angle) * ( clockRadius - ( clockRadius / 8 ) ) ) );
    display->drawLine( x2 + x , y2 + y , x3 + x , y3 + y);
  }

  // display second hand
  float angle = timeClient.getSeconds() * 6 ;
  angle = ( angle / 57.29577951 ) ; //Convert degrees to radians
  int x3 = ( clockCenterX + ( sin(angle) * ( clockRadius - ( clockRadius / 5 ) ) ) );
  int y3 = ( clockCenterY - ( cos(angle) * ( clockRadius - ( clockRadius / 5 ) ) ) );
  display->drawLine( clockCenterX + x , clockCenterY + y , x3 + x , y3 + y);
  //
  // display minute hand
  angle = timeClient.getMinutes() * 6 ;
  angle = ( angle / 57.29577951 ) ; //Convert degrees to radians
  x3 = ( clockCenterX + ( sin(angle) * ( clockRadius - ( clockRadius / 4 ) ) ) );
  y3 = ( clockCenterY - ( cos(angle) * ( clockRadius - ( clockRadius / 4 ) ) ) );
  display->drawLine( clockCenterX + x , clockCenterY + y , x3 + x , y3 + y);
  //
  // display hour hand
  angle = timeClient.getHours() * 30 + int( ( timeClient.getMinutes() / 12 ) * 6 )   ;
  angle = ( angle / 57.29577951 ) ; //Convert degrees to radians
  x3 = ( clockCenterX + ( sin(angle) * ( clockRadius - ( clockRadius / 2 ) ) ) );
  y3 = ( clockCenterY - ( cos(angle) * ( clockRadius - ( clockRadius / 2 ) ) ) );
  display->drawLine( clockCenterX + x , clockCenterY + y , x3 + x , y3 + y);
}
/******** 数字时钟显示内容**********/
void digitalClockFrame(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
   timeClient.update();
 // String timenow = String(timeClient.getHours()) + ":" + twoDigits(timeClient.getMinutes()) + ":" + twoDigits(timeClient.getSeconds());
 // display->setTextAlignment(TEXT_ALIGN_CENTER);
 display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_24);
   display->drawString(16 + x, 28 + y, timeClient.getFormattedTime()); //显示位置
 // display->drawString(clockCenterX + x , clockCenterY + y, timenow );
   display->drawXbm(42, 0, 16, 16, xing);//星期显示
  display->drawXbm(59,0, 16, 16, qi); 
 display->drawXbm(76, 0, 16, 16, Week[timeClient.getDay()]);
 display->drawHorizontalLine(0, 18, 128); //画水平线
}

// This array keeps function pointers to all frames
// frames are the single views that slide in
FrameCallback frames[] = { analogClockFrame, digitalClockFrame };

// how many frames are there?
int frameCount = 2;

// Overlays are statically drawn on top of a frame eg. a clock
//叠加是静态绘制在框架上的。一个时钟
//OverlayCallback overlays[] = { clockOverlay };
//int overlaysCount = 1;

void setup() {
  Serial.begin(115200);
  Serial.println();
    WiFi.begin(ssid, password);
while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  
  // The ESP is capable of rendering 60fps in 80Mhz mode
  // but that won't give you much time for anything else
  // run it in 160Mhz mode or just set it to 30 fps
  ui.setTargetFPS(30);

  // Customize the active and inactive symbol
 // ui.setActiveSymbol(activeSymbol);
 // ui.setInactiveSymbol(inactiveSymbol);//页码图标
  ui.disableAllIndicators();             //不显示页码小点。
  // You can change this to
  // TOP, LEFT, BOTTOM, RIGHT
 // ui.setIndicatorPosition(TOP);

  // Defines where the first frame is located in the bar.
  ui.setIndicatorDirection(LEFT_RIGHT);

  // You can change the transition that is used
  // SLIDE_LEFT, SLIDE_RIGHT, SLIDE_UP, SLIDE_DOWN
  ui.setFrameAnimation(SLIDE_LEFT);

// 设置框架和显示屏幕内容数
  ui.setFrames(frames, frameCount);

  // 设置覆盖的画面数
//  ui.setOverlays(overlays, overlaysCount);

  // Initialising the UI will init the display too.
  ui.init();

  display.flipScreenVertically();
  timeClient.begin();
//  unsigned long secsSinceStart = millis();
  // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
//  const unsigned long seventyYears = 2208988800UL;
  // subtract seventy years:
//  unsigned long epoch = secsSinceStart - seventyYears * SECS_PER_HOUR;
//  setTime(epoch);

}


void loop() {
  int remainingTimeBudget = ui.update();

  if (remainingTimeBudget > 0) {
    // You can do some work here
    // Don't do stuff if you are below your
    // time budget.
    delay(remainingTimeBudget);
  }
}
