/*
 * 
 * 中断触发模式
LOW               低电平触发
CHANGE            电平变化，高电平变低电平、低电平变高电平
RISING            上升沿触发
FALLING           下降沿触发
 */

#define timeSeconds 10  //触发中断时，停留时间参数


const int led = 12;//D6
// 设置外部中断输入信号的引脚
const int Sensor = 14;//D5

// 时间函数
unsigned long now = millis();
unsigned long lastTrigger = 0;
bool startTimer = false;

// 当触发中断时，运行此函数
ICACHE_RAM_ATTR void Interrupt_function() {
  Serial.println("The trigger!!!");
  digitalWrite(led, LOW);
  startTimer = true;
  lastTrigger = millis();
}

void setup() {
  Serial.begin(115200);
  
  //  设置外部中断信号输入引脚模式 INPUT_PULLUP
  pinMode(Sensor, INPUT_PULLUP);
  // 设置外部中断引脚,中断触发模式 RISING ，上升沿触发
  attachInterrupt(digitalPinToInterrupt(Sensor), Interrupt_function,RISING);

  // Set LED to LOW
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);
}

void loop() {  
  now = millis();// 当前时间
  // 当触发中断时，运行任务函数的时间
  if(startTimer && (now - lastTrigger > (timeSeconds*1000))) {
    Serial.println("close...");
    digitalWrite(led, HIGH);
    startTimer = false;
  }
}
