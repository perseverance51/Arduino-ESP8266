#include <PWM.h>
//库下载地址：https://github.com/abrightwell/arduino-pwm-frequency-library
int led = 9;                // led所连接到的引脚
int brightness = 100;         // led的亮度，也就是占空比，范围是0-255
int32_t frequency = 10000; // 频率 (单位是Hz)

void setup()
{
  // 初始化除了0号计时器以外的其他计时器
  InitTimersSafe(); 

  // 设置指定引脚的频率
  bool success = SetPinFrequencySafe(led, frequency);
  
}

void loop()
{
  pwmWrite(led, brightness);
  delay(30);      
}
