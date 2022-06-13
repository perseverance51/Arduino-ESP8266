
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
 
//以苏宁易购api为例子获取时间数据
#define SERVER_IP "http://quan.suning.com/getSysTime.do"
 
 
//设置你的wifi接入信息
#ifndef STASSID
#define STASSID "MERCURY_D268G"
#define STAPSK  "pba5ayzk"
#endif
 
void setup() {
 
  //初始化串口设置
  Serial.begin(115200);
  Serial.println();
 
  //开始连接wifi
  WiFi.begin(STASSID, STAPSK);
 
  //等待WiFi连接,连接成功打印IP
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
 
}
 
void loop() {
  if ((WiFi.status() == WL_CONNECTED)) {
    http_post();
  }
  delay(10000);
}
 
void http_post() {
 
  //创建 WiFiClient 实例化对象
  WiFiClient client;
 
  //创建http对象
  HTTPClient http;
 
  //配置请求地址
  http.begin(client, SERVER_IP); //HTTP请求
  Serial.print("[HTTP] begin...\n");
 
  //启动连接并发送HTTP报头和报文
  int httpCode = http.POST(" ");
  Serial.print("[HTTP] POST...\n");
 
  //连接失败时 httpCode时为负
  if (httpCode > 0) {
 
    //将服务器响应头打印到串口
    Serial.printf("[HTTP] POST... code: %d\n", httpCode);
 
    //将从服务器获取的数据打印到串口
    if (httpCode == HTTP_CODE_OK) {
      const String& payload = http.getString();
      Serial.println("received payload:\n<<");
      Serial.println(payload);
      Serial.println(">>");
    }
  } else {
    Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  //关闭http连接
  http.end();
}
