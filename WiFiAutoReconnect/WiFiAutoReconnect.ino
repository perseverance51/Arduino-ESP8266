#include <ESP8266WiFi.h>

// 替换为自己的WIFI
const char* ssid = "MERCURY_D268G";
const char* password = "pba5ayzk";

unsigned long previousMillis = 0;
unsigned long interval = 30000;

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  WiFi.setAutoReconnect(true);  //ESP8266会在连接断开时自动重新连接
  WiFi.persistent(true);//自动重新连接到以前连接的接入点
}

void setup() {
  Serial.begin(115200);
  initWiFi();
  Serial.print("RSSI: ");
  Serial.println(WiFi.RSSI());
}

void loop() {
  //每30秒打印一次Wi-Fi状态
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >=interval){
    switch (WiFi.status()){
      case WL_NO_SSID_AVAIL:
        Serial.println("无法连接到已配置的SSID");
        break;
      case WL_CONNECTED:
        Serial.println("成功建立连接");
        break;
      case WL_CONNECT_FAILED:
        Serial.println("连接失败");
        break;
    }
    Serial.printf("Connection status: %d\n", WiFi.status());
    Serial.print("RRSI: ");
    Serial.println(WiFi.RSSI());
    previousMillis = currentMillis;
  }
}
