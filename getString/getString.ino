/**********************************************************************
  此程序用于演示如何使用getString来处理气象网站服务器返回的响应信息。
  // HTTPClient库用于HTTP协议通讯。通过ESP8266HTTPClient库，
// 我们可以使用ESP8266利用互联网或局域网向网络服务器发送HTTP请求，
// 并且分析网络服务器返回的HTTP响应信息，从而实现物联网应用。
// TCP/IP
// ESP8266库中还有一个专门用于TCP通讯的WiFiClient库。由于HTTP协议是建立在TCP协议基础之上的，
// 我们也可以使用WiFiClient库来实现HTTP通讯。在这一点上，ESP8266HTTPClient库与WiFiClient库在功能上形成了互补。
 申明对象
 WiFiClient client;
    HTTPClient http;
***********************************************************************/


#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
 
// 测试HTTP请求用的URL
#define URL "http://www.weather.com.cn/data/sk/101250301.html"
 
// 设置wifi接入信息(请根据您的WiFi信息进行修改)
const char* ssid = "MERCURY_D268G";
const char* password = "pba5ayzk";
 
void setup() {
  //初始化串口设置
  Serial.begin(115200);
 
  //设置ESP8266工作模式为无线终端模式
  WiFi.mode(WIFI_STA);
 
  //开始连接wifi
  WiFi.begin(ssid, password);
 
  //等待WiFi连接,连接成功打印IP
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi Connected!");
}
 
void loop() {
  // 如果ESP8266连接WiFi则发送HTTP请求
  if ((WiFi.status() == WL_CONNECTED)) {
    esp8266Http();
  }
  
  delay(8000);   // 短暂等待
}
 
// 发送HTTP请求并且将服务器响应通过串口输出
void esp8266Http(){
 
  //创建 HTTPClient 对象
  HTTPClient httpClient;
 
  //配置请求地址。此处也可以不使用端口号和PATH而单纯的
  httpClient.begin(URL); 
  Serial.print("URL: "); Serial.println(URL);
 
  //启动连接并发送HTTP请求
  int httpCode = httpClient.GET();
  Serial.print("Send GET request to URL: ");
  Serial.println(URL);
  
  //如果服务器响应OK则从服务器获取响应体信息并通过串口输出
  //如果服务器不响应OK则将服务器响应状态码通过串口输出
  if (httpCode == HTTP_CODE_OK) {
    String responsePayload = httpClient.getString();
    Serial.println("Server Response Payload: ");
    Serial.println(responsePayload);
  } else {
    Serial.println("Server Respose Code：");
    Serial.println(httpCode);
  }
 
  //关闭ESP8266与服务器连接
  httpClient.end();
}
