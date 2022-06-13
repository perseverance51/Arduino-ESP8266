
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>//点击这里会自动打开管理库页面: http://librarymanager/All#LiquidCrystal_I2C


LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup()
{
  lcd.init();
  lcd.backlight();
  lcd.setCursor(2,0);//在第一行的第二个字符开始显示内容
  lcd.print("Hello, world!");
  lcd.setCursor(2,1);//在第二行的第二个字符开始显示字符
  lcd.print("Perseverance");
}


void loop()
{
}
