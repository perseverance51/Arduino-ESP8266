/* @file CustomKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates changing the keypad size and key values.
|| #
    Use with I2C i/o G. D. (Joe) Young Feb 28/12
*/
#include <Keypad_I2C.h>
#include <Keypad.h>        // GDY120705
#include <Wire.h>
#include <LiquidCrystal_I2C.h>//点击这里会自动打开管理库页面: http://librarymanager/All#LiquidCrystal_I2C
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
#define I2CADDR 0x21

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'0','1','2','3'},
  {'4','5','6','7'},
  {'8','9','A','B'},
  {'C','D','E','F'}
};
byte rowPins[ROWS] = {3, 2, 1, 0}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {7, 6, 5, 4}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad_I2C customKeypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS, I2CADDR); 

void setup(){
  Wire.begin();                // GDY200622
  customKeypad.begin( );        // GDY120705
    lcd.init();
  lcd.backlight();
  lcd.setCursor(2,0);//在第一行的第二个字符开始显示内容
  lcd.print("Hello, world!");
  lcd.setCursor(2,1);//在第二行的第二个字符开始显示字符
  lcd.print("Perseverance");
  Serial.begin(9600);
}
  
void loop(){
  char customKey = customKeypad.getKey();
  
  if (customKey != NO_KEY){
    Serial.println(customKey);
    
  }
}
