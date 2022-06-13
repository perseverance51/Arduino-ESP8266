#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "index.h"
#define LED  2
// wifi信息
const char* ssid = "MERCURY_D268G";
const char* password = "pba5ayzk";

String s ="";
ESP8266WebServer server(80);
//WiFiServer server(80);
void handleRoot() 
{
  s = webpage;
  delay(200);
 server.send(200, "text/html", s);
}

void sensor_data() 
{
 int a = analogRead(A0);
 int temp= a;
 String sensor_value = String(temp);
 server.send(200, "text/plane", sensor_value);
}
void led_control() 
{
 String state = "OFF";
 String act_state = server.arg("state");
 if(act_state == "1")
 {
  digitalWrite(LED,HIGH); //LED ON
  state = "ON";
 }
 else
 {
  digitalWrite(LED,LOW); //LED OFF
  state = "OFF";
 }
 server.send(200, "text/plane", state);
}
void setup() {
  Serial.begin(115200);
   WiFi.mode(WIFI_STA);
   pinMode(LED_BUILTIN, OUTPUT);//led灯设置为输出
 WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED)
  {
        Serial.print('.');
        delay(500);
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.on("/", handleRoot);
  server.on("/led_set", led_control);
  server.on("/adcread", sensor_data);
  server.begin();
}

void loop() {
  server.handleClient();
    delay(500);
}
