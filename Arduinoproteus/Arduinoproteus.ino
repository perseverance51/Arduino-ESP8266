/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystalHelloWorld

*/

// include the library code:
#include <LiquidCrystal.h>
#include <stdio.h>
#include <DS1302.h>


// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
DS1302 rtc(8, 7, 6); //对应DS1302的RST,DAT,CLK

void printTime()//打印时间数据
{
  Time tim = rtc.time(); //从DS1302获取时间数据
  char buf[22];
  snprintf(buf, sizeof(buf), "%02d-%02d-%02d %02d:%02d",
           tim.yr, tim.mon, tim.date,
           tim.hr, tim.min);
//获取星期时，需要做减一计算，由前面的星期映射关系决定的
  lcd.setCursor(0, 1);//光标定位 ,

    lcd.print(buf);
    lcd.setCursor(15, 0);
    lcd.print(tim.day-1);
    
}


void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
    rtc.writeProtect(false);
      for (int i = 0; i < DS1302::kRamSize; ++i) {
    rtc.writeRam(i, 0x00);
  }
  // Print a message to the LCD.
  lcd.setCursor(0, 0);//光标定位
  lcd.print("Perseverance51");
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  printTime();
 delay(1000);
  // print the number of seconds since reset:
 // lcd.print(millis() / 1000);
}
