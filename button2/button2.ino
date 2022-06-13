#include <Arduino.h>
#include "Button2.h"

int pin_num = 5;     //按键的引脚号
Button2 button = Button2(pin_num);
void handler(Button2 &btn);
void setup()
{
  //pinMode(pin_num, INPUT);  //按键设置为输入模式
  Serial.begin(115200); //波特率设置
  button.setClickHandler(handler);  //设置四种按法的回调函数
  button.setLongClickHandler(handler);
  button.setDoubleClickHandler(handler);
  button.setTripleClickHandler(handler);
}

void loop()
{
  button.loop();  //button的loop，参照官方例程

}
void handler(Button2& btn) {  //回调函数，官方例程写法，代码很通俗不过多注释了
    switch (btn.getClickType()) {
        case SINGLE_CLICK:    //判断按键方式，在里面编辑自己的命令
            Serial.print("single ");  //串口发送
            break;
        case DOUBLE_CLICK:
            Serial.print("double ");
            break;
        case TRIPLE_CLICK:
            Serial.print("triple ");
            break;
        case LONG_CLICK:
            Serial.print("long");
            break;
    }
    Serial.print("click");
    Serial.print(" (");
    Serial.print(btn.getNumberOfClicks());    
    Serial.println(")");
}
