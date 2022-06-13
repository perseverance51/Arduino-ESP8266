/****发射端***/
#include <RH_ASK.h>
#include <SPI.h> // 不实际使用，但需要编译


RH_ASK driver;

void setup()
{
    Serial.begin(9600);    // Debugging only
    if (!driver.init())
         Serial.println("init failed");
}

void loop()
{
    const char *msg = "perseverance";
    driver.send((uint8_t *)msg, strlen(msg));
    driver.waitPacketSent();
    
    delay(1000);
    Serial.println("send date！");
}
