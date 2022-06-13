#define Pin_Led D7
void setup() { 
 pinMode(Pin_Led,OUTPUT); //设置引脚模式
 analogWrite(Pin_Led,0); 
 //PWM频率范围应该是100Hz-40KHz,默认是1kHZ
 analogWriteFreq(100);//100 -  40000
 }
 void loop() { 
  for(int val=0;val<1024;val++){ 
    analogWrite(Pin_Led,val); //占空比逐渐增大,亮度渐亮
    delay(25); 
    }
    for(int val=1023;val>=0;val--){ 
      analogWrite(Pin_Led,1023); 
      delay(25); //占空比逐渐变小,亮度渐暗
      }
      }
