/**
 * More examples and docs see :
 * https://github.com/mcxiaoke/ESPDateTime
 *
 */
#include <Arduino.h>
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif
#include "ESPDateTime.h"

/*填写wifi信息*/
/*填写wifi信息*/
const char *ssid= "MERCURY_D268G";
const char *password = "pba5ayzk";

unsigned long lastMs = 0;
unsigned long ms = millis();

void setup() {
  Serial.begin(115200);
    delay(1000);
  WiFi.begin(ssid, password);//配网
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("IP number assigned by DHCP is ");
  Serial.println(WiFi.localIP());
  DateTime.setServer("time.pool.aliyun.com");
  DateTime.setTimeZone("CST-8");
}

void loop() {
  if (millis() - ms > 15 * 1000L) {
    ms = millis();
    Serial.println("--------------------");
    Serial.println(DateTime.now());//时间戳1655809168
    Serial.println("--------------------");
  Serial.println(DateTime.toString());//2022-06-21 10:59:28
  Serial.println("--------------------");
  Serial.println(DateTime.format(DateFormatter::COMPAT));//20220621_105928
  Serial.println(DateTime.format(DateFormatter::DATE_ONLY));//2022-06-21
  Serial.println(DateTime.format(DateFormatter::TIME_ONLY));//10:59:28
 Serial.println("--------------------");
  DateTimeParts p = DateTime.getParts();
  Serial.printf("%04d/%02d/%02d %02d:%02d:%02d %ld \n", p.getYear(),
                p.getMonth(), p.getMonthDay(), p.getHours(), p.getMinutes(),
                p.getSeconds(), p.getTime());//2022/05/21 11:06:57 1655809617 
  Serial.println("--------------------");
    if (!DateTime.isTimeValid()) {
      Serial.println("Failed to get time from server, retry.");
      DateTime.begin();
    } 
  }
}
