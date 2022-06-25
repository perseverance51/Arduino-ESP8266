/*
 * 
 * ESPNtpClient；https://github.com/gmag11/ESPNtpClient
 * 只能编译esp8266
 */

#include <ESPNtpClient.h>
#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif

//填写wifi信息
#define YOUR_WIFI_SSID "MERCURY_D268G"
#define YOUR_WIFI_PASSWD "pba5ayzk"

//时间刷新频率
#define SHOW_TIME_PERIOD 1000

void setup() {
    Serial.begin (115200);
    Serial.println ();
    WiFi.begin (YOUR_WIFI_SSID, YOUR_WIFI_PASSWD);
      while ( WiFi.status() != WL_CONNECTED ) {//没有入网情况下在这里运行
    delay ( 500 );
    Serial.print ( "." );
  }
    NTP.setTimeZone (TZ_Asia_Hong_Kong);//时区
    NTP.begin ();
}

void loop() {
    static int last = 0;
    if ((millis () - last) > SHOW_TIME_PERIOD) {
        last = millis ();
        Serial.println (NTP.getTimeDateString());//21/06/2022 11:31:53
      //Serial.println (NTP.getTimeDateStringUs());//21/06/2022 03:10:24.953254 UTC

    }
}
