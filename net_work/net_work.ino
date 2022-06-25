/*
 * 国内NTP服务器地址
 * ntp1.aliyun.com - ntp7.aliyun.com可选
 * cn.ntp.org.cn
 * 
 */
#include <NTPClient.h>//点击这里会自动打开管理库页面: http://librarymanager/All#NTPClient
#ifdef ESP8266
#include <ESP8266WiFi.h>
#elif  ESP32
#include <WiFi.h> // for WiFi shield
#elif  MKR1000
#include <WiFi101.h> // for WiFi 101 shield or MKR1000
#endif
#include <WiFiUdp.h>
/*填写wifi信息*/
const char *ssid= "MERCURY_D268G";
const char *password = "pba5ayzk";

WiFiUDP ntpUDP;
// NTPClient(UDP &udp, const char *poolServerName, int timeOffset);
//NTPClient timeClient(ntpUDP);//默认NTP：time.nist.gov，
//NTPClient timeClient(ntpUDP,60*60*8);//添加偏移时间
NTPClient timeClient(ntpUDP,"ntp3.aliyun.com",60*60*8);//timeOffset：0,国内属于东8区,以秒为偏移单位
// NTPClient(UDP &udp, const char *poolServerName, int timeOffset, int updateInterval);//updateInterval:自定义(在测试过程中设置了不见效果)
void setup(){
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED ) {//没有入网情况下在这里运行
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();
}

void loop() {
  timeClient.update();
  Serial.println(timeClient.getFormattedTime());
  delay(1000);
}
