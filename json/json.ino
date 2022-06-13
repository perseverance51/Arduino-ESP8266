
//needed for library
//#include <DNSServer.h>
//#include <ESP8266WebServer.h>
//#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
//Web配网启用上面3个库
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <ArduinoJson.h>


// 设置wifi接入信息(请根据您的WiFi信息进行修改)
const char* ssid = "MERCURY_D268G";
const char* password = "pba5ayzk";

void setup() {
    // put your setup code here, to run once:
   Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
//    WiFiManager wifiManager;
//    wifiManager.autoConnect("AutoConnectAP");
//    Serial.println("connected...yeey :)");
 // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println(WiFi.localIP());

}

void loop() {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  delay(20000);
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  client->setInsecure();
  HTTPClient http;
String url = "https://web.ifzq.gtimg.cn/appstock/app/minute/query?code=sz000001";//
 Serial.print("[HTTP] begin...\n");
  if (http.begin(*client, url)) {  // HTTP
    Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();
    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: % d\n", httpCode);
      // file found at server
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String json = http.getString();
      Serial.println(json);
    
}
    }
  }
}
