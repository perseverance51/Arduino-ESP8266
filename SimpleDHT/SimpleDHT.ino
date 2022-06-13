
#include <ESP8266WiFi.h>
#include <SimpleDHT.h>//点击这里会自动打开管理库页面: http://librarymanager/All#SimpleDHT
int pinDHT11 = D7;
SimpleDHT11 dht11(pinDHT11);
 
const char* ssid = "MERCURY_D268G";
const char* password = "pba5ayzk";

WiFiServer server(80);
 
void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);//板载led
  digitalWrite(2, 0);
 
  Serial.println();         // Connect to WiFi network
  Serial.println();
  Serial.print("Connecting to "); Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print(".");}
  Serial.println("");
  Serial.println("WiFi connected");
  server.begin();
  Serial.println(WiFi.localIP());
}
 
void loop() {
  WiFiClient client = server.available();// Check if a client has connected
  if (!client) { delay(2000);return;}  
  Serial.println("=========================");
  byte temperature = 0;
  byte humidity    = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read TEM-HUM failed, err="); Serial.println(err);delay(2000);
    return;}
  Serial.print("DHT11 ~ TEM-HUM : ");
  Serial.print((int)temperature); Serial.print("℃, "); 
  Serial.print((int)humidity); Serial.println("%");
  delay(2000);
  
  client.flush();
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n ";
  s+="<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta http-equiv=\"Refresh\" content=\"5\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, minimum-scale=0.5, maximum-scale=2.0, user-scalable=yes\" /><meta http-equiv=\"X-UA-Compatible\" content=\"ie=edge\"><title>温湿度数据显示</title><style>  h2,h1{line-height:1%;}body {  margin: 0;padding: 0;width: 340px;  background: LightCyan}.button { width: 100px; height: 100px;  text-align: center; font-weight: 100; color: darkcyan;  margin: 0 40px 40px 0;  position: relative; border: 6px solid darkcyan; background: LightCyan;  font-size: 20px;  border-radius: 50%;}.top1 { width: 360px; height: 45px; color: #FFF;  border: 1px solid darkcyan; background: darkcyan; font-size: 25px;  border-radius: 0%;}</style></head><body><a href=\"./pin?light1=1\"><button class=\"button top1\">&#x6E29;&#x6E7F;&#x5EA6;&#x663E;&#x793A;</button></a><center style=\"left: 20px; position: relative;\"></br><a href=\"./pin?light1=1\"><button type=\"button\" class=\"button\" value=\"temp\">温度<span style=\"color: red;font-size: 25px;\">";

  s += ((int)temperature);
  s += "°C</span></button></a><a href=\"./pin?light1=0\"><button type=\"button\" class=\"button\" value=\"humi\">湿度<span style=\"color: green;font-size: 25px;\">";
  
  s += ((int)humidity);
  s += "%</span></button></a></br></center></body></html>";
  client.print(s); delay(1);  
} 
