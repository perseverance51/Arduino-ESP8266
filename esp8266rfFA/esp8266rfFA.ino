// ask_transmitter.pde
// -*- mode: C++ -*-
// Simple example of how to use RadioHead to transmit messages
// with a simple ASK transmitter in a very simple way.
// Implements a simplex (one-way) transmitter with an TX-C1 module
// Tested on Arduino Mega, Duemilanova, Uno, Due, Teensy, ESP-12

#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // 没有实际使用，但需要编译
#endif

//RH_ASK driver;
// 2000 bps, recieve on GPIO2--D4, transmit on GPIO4--D2, PTT on GPIO5--D1
RH_ASK driver(2000, 2, 4, 5); // ESP8266 or ESP32: do not use pin 11 or 2
// RH_ASK driver(2000, 3, 4, 0); // ATTiny, RX on D3 (pin 2 on attiny85) TX on D4 (pin 3 on attiny85),
// RH_ASK driver(2000, PD14, PD13, 0); STM32F4 Discovery: see tx and rx on Orange and Red LEDS

void setup()
{
  Serial.begin(9600);    // Debugging only
  if (!driver.init())
    Serial.println("init failed");
  pinMode(D2, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  Serial.println("Send Port");
}

void loop()
{
  const char *msg = "perseverance";
  driver.send((uint8_t *)msg, strlen(msg));
  driver.waitPacketSent();
  delay(2000);
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}
