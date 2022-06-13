/*

  U8g2Logo.ino

  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

*/

#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

/*
  U8g2lib Example Overview:
    Frame Buffer Examples: clearBuffer/sendBuffer. Fast, but may not work with all Arduino boards because of RAM consumption
    Page Buffer Examples: firstPage/nextPage. Less RAM usage, should work with all Arduino boards.
    U8x8 Text Only Example: No RAM usage, direct communication with display controller. No graphics, 8x8 Text only.
    
  This is a frame buffer example.    
*/

// Please UNCOMMENT one of the contructor lines below
// U8g2 Contructor List (Frame Buffer)
// The complete list is available here: https://github.com/olikraus/u8g2/wiki/u8g2setupcpp
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);//0.96寸
//U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);//1.3寸


// End of constructor list

//#define MINI_LOGO
void drawBattery(U8G2 u8g2, int x, int y, int w, int h, int segments, int lvl) {//x横坐标，y纵坐标，w
    u8g2.drawFrame(x,y,w,h);
    u8g2.drawFrame(x + w / 3, y - 2, w / 3, 2);
    
    for(int i = 0; i < segments; i++) {
        if ((segments -i) > lvl) {
        //    u8g2.drawFrame(x+2, y + i*h/segments + 2, w-4, h/(segments+1) - 1);
        } else {
            u8g2.drawBox(x+2, y + i*h/segments +2, w-3, h/(segments+1) - 1);//画实心方形
        }
    }
}
void drawBattery2(U8G2 u8g2, int x, int y, int w, int h, int segments, int lvl) {//x横坐标，y纵坐标，w
    u8g2.drawFrame(x,y,w,h);
    u8g2.drawFrame(x - 2, y + h /4, h / 6, 5);
    
    for(int i = 0; i < segments; i++) {
        if ((segments -i) > lvl) {
        //    u8g2.drawFrame(x+2, y + i*h/segments + 2, w-4, h/(segments+1) - 1);
        } else {
            u8g2.drawBox(x+ i*w/segments+2, y+2, w/(segments+1) - 1, h-3);//画实心方形
        }
    }

}
void drawSignal(U8G2 u8g2, uint8_t x, uint8_t y, uint8_t strength) {
    for (uint8_t i = 0; i < strength; i++) {
        u8g2.drawCircle(x,y,i*3, U8G2_DRAW_UPPER_RIGHT);
    }
}


void draw() {
 static uint8_t counter = 1; 
 counter %=6;
    u8g2.setDrawColor(1);
    u8g2.setFont(u8g2_font_5x8_tr);
//    u8g2.drawStr(2,12,"Battery: ");
//    u8g2.drawStr(2,44,"Signal: ");
//    drawBattery(u8g2, 70,4,12,20,5,counter%6);
//    drawSignal(u8g2, 70,46,counter%6);
    drawBattery(u8g2, 2,20,12,20,5,counter%6);
    drawSignal(u8g2, 0,12,counter%6);
 drawBattery2(u8g2, 107,0,20,12,5,counter%6); 
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(50, 8 , 0xe242 + counter%10);//绘制字体字集的符号表示电池充电
    
        counter ++;
}

void setup(void) {
  pinMode(2, OUTPUT);
  digitalWrite(2, 0);	
  // default output in I2C mode for the SSD1306 test shield: set the i2c adr to 0
  u8g2.begin();
}


void loop(void) {
  u8g2.clearBuffer();
  draw();
   u8g2.sendBuffer();
    delay(1000);
}
