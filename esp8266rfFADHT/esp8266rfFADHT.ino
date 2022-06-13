// ask_transmitter.pde
// -*- mode: C++ -*-
// Simple example of how to use RadioHead to transmit messages
// with a simple ASK transmitter in a very simple way.
// Implements a simplex (one-way) transmitter with an TX-C1 module
// Tested on Arduino Mega, Duemilanova, Uno, Due, Teensy, ESP-12

#include <RH_ASK.h>
#include "DHT.h";
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // 没有实际使用，但需要编译
#endif

#define DHTPIN 13       //GPIO13--D7 DHT-11 Output Pin connection
#define DHTTYPE DHT11

float hum;    // Stores humidity value in percent
float temp;
String str_humid;
String str_temp;
String str_out;
DHT dht(DHTPIN, DHTTYPE);

//RH_ASK driver;
// 2000 bps, recieve on GPIO2--D4, transmit on GPIO4--D2, PTT on GPIO5--D1
RH_ASK driver(2000, 2, 4, 5); // ESP8266 or ESP32: do not use pin 11 or 2
// RH_ASK driver(2000, 3, 4, 0); // ATTiny, RX on D3 (pin 2 on attiny85) TX on D4 (pin 3 on attiny85),
// RH_ASK driver(2000, PD14, PD13, 0); STM32F4 Discovery: see tx and rx on Orange and Red LEDS

void setup()
{
    dht.begin();
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
  //const char *msg = "perseverance";
  hum = dht.readHumidity();  // Get Humidity value
  temp = dht.readTemperature();
  delay(500);  // Delay so DHT-11 sensor can stabalize
  // 湿度数据转换
  str_humid = String(hum);
  // 温度数据转换
  str_temp = String(temp);
  // 将湿度和温度拼接成一体
  str_out = "RH："+ str_humid +"%"+ "," +"Temp:"+ str_temp + "℃";
  // 转换为RF射频模块发送数据类型
  const char *msg = str_out.c_str();
 
  driver.send((uint8_t *)msg, strlen(msg));
  driver.waitPacketSent();
  delay(2000);
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}
