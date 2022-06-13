// ask_receiver.pde
// -*- mode: C++ -*-
// Simple example of how to use RadioHead to receive messages
// with a simple ASK transmitter in a very simple way.
// Implements a simplex (one-way) receiver with an Rx-B1 module
// Tested on Arduino Mega, Duemilanova, Uno, Due, Teensy, ESP-12

#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // 没有实际使用，但需要编译
#endif

//RH_ASK driver;
// 2000 bps, recieve on GPIO 13--D7, transmit on GPIO4--D2, PTT on GPIO5--D1
RH_ASK driver(2000, 13,4, 5); 
// RH_ASK driver(2000, 4, 5, 0); // ESP8266 or ESP32: do not use pin 11 or 2
// RH_ASK driver(2000, 3, 4, 0); // ATTiny, RX on D3 (pin 2 on attiny85) TX on D4 (pin 3 on attiny85), 
// RH_ASK driver(2000, PD14, PD13, 0); STM32F4 Discovery: see tx and rx on Orange and Red LEDS

void setup()
{

  Serial.begin(9600);
  if (!driver.init())
    Serial.println("init failed");
  pinMode(D7,INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  Serial.println("Revice Port");
}

void loop()
{
    uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];//最大长度时67，定义在RH_ASK.h文件中
    uint8_t buflen = sizeof(buf);
    if (driver.recv(buf, &buflen)) // Non-blocking
    {

  // Message with a good checksum received, dump it.
//  driver.printBuffer("Got:",buf, buflen);//这个会打印以16进制打印出来，所以不启用
  Serial.print("Message: ");
  Serial.println((char*)buf);  
  memset(buf, 0, sizeof(buf));//清空接收的数据，方便下一次接收存储 
 // strcpy(buf, "");
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }
}
