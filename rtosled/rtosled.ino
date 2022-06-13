#include <Arduino_FreeRTOS.h>
#define LED1  9
#define LED2  6
#define LED3  10
void sdelay(unsigned char a){
  volatile unsigned char i,j,k;
  for(i=0;i<a;i++)
    for(j=0;j<200;j++)
      for(k=0;k<150;k++)
          ;
  }
void vTask1(void *pvParameters){
  pinMode(LED1,OUTPUT);
  for(;;)
    {
     digitalWrite(LED1,HIGH);
    // sdelay(20);
     vTaskDelay(50); 
     digitalWrite(LED1,LOW);
    // sdelay(20);
     vTaskDelay(50); 
    }
}
void vTask2(void *pvParameters){
  pinMode(LED2,OUTPUT);
  for(;;)
    {
     digitalWrite(LED2,HIGH);
    // sdelay(20);
     vTaskDelay(100); 
     digitalWrite(LED2,LOW);
    // sdelay(20);
    vTaskDelay(100); 
    }
}
void vTask3(void *pvParameters){
  pinMode(LED3,OUTPUT);
  for(;;)
    {
     digitalWrite(LED3,HIGH);
     //sdelay(20);
     vTaskDelay(75); 
     digitalWrite(LED3,LOW);
    // sdelay(20);
    vTaskDelay(75); 
    }
}
void setup() {
  xTaskCreate(vTask1,NULL,64,NULL,1,NULL);
  xTaskCreate(vTask2,NULL,64,NULL,1,NULL);
  xTaskCreate(vTask3,NULL,64,NULL,1,NULL);
}
void loop(){
}
