/**********************************************************************

  ESP8266WiFiMulti

***********************************************************************/
 
#include "ESP8266WiFiMulti.h"
 
ESP8266WiFiMulti WiFiMulti;//实例化ESP8266WiFiMulti对象
 
void setup() {
  Serial.begin(115200);
  delay(100);
 
  //通过addAp函数存储  WiFi名称       WiFi密码
    WiFiMulti.addAP("MERCURY_D268G", "pba5ayzk");//添加路由器WIFI网络，注意：这里不是创建WIFI热点
      WiFiMulti.addAP("CMCC_DyVv", "pba5ayzk");
//      WiFiMulti.addAP("MERCURY_D268G", "pba5ayzk");
  // 这2条语句通过调用函数addAP来记录2个不同的WiFi网络信息。
  // 这2个WiFi网络名称分别是MERCURY_D268G, CMCC_DyVv, 
  // 这2个网络的密码都是pba5ayzk
  // 此处WiFi信息只是示例，请在使用时将需要连接的WiFi信息填入相应位置。
  // 另外这里只存储了3个WiFi信息，您可以存储更多的WiFi信息在此处。
    
  while(WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  } 
  // 此处的wifiMulti.run()是重点。通过wifiMulti.run()，NodeMCU将会在当前
  // 环境中搜索addAP函数所存储的WiFi。如果搜到多个存储的WiFi那么NodeMCU
  // 将会连接信号最强的那一个WiFi信号。
  // 一旦连接WiFI成功，wifiMulti.run()将会返回“WL_CONNECTED”。这也是
  // 此处while循环判断是否跳出循环的条件。
 
  Serial.println('\n');                     // WiFi连接成功后
  Serial.print("Connected to ");            // NodeMCU将通过串口监视器输出。
  Serial.println(WiFi.SSID());              // 连接的WiFI名称
  Serial.print("IP address:\t");            // 以及
  Serial.println(WiFi.localIP());           // NodeMCU的IP地址
 Serial.println(String("IP 地址是: ") + String(WiFi.softAPIP().toString()));//这个打印的是开发板STA和AP兼容模式创建WebServer服务对外开启的网关地址，可以接入这个AP网络进行通信
  
}
 
void loop() {}
