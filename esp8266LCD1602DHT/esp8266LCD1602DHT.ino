/****驱动LCD1602转IIC屏幕****/
#include <LiquidCrystal_I2C.h>//点击这里会自动打开管理库页面: http://librarymanager/All#LiquidCrystal_I2C
LiquidCrystal_I2C lcd(0x27, 16, 2); // 屏幕硬件地址和屏幕参数每行16个字符，2行
 /****采集温度传感器****/
#include <OneWire.h> 
#include <DallasTemperature.h>//点击这里会自动打开管理库页面: http://librarymanager/All#DallasTemperature
#define ONE_WIRE_BUS 14 //D5
OneWire oneWire(ONE_WIRE_BUS); // 初始连接在单总线上的单总线设备
DallasTemperature sensors(&oneWire); 
  /****创建网页服务****/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
 
/*
// NETWORK: Static IP details...
IPAddress ip(192, 168, 1, 10); // a valid IP on your network
IPAddress gateway(192, 168, 1, 1); //IP of your router / gateway
IPAddress subnet(255, 255, 255, 0); //subnet mask of your network
*/
 
// wifi信息
const char* ssid = "MERCURY_D268G";
const char* password = "pba5ayzk";
ESP8266WebServer server(80); //instantiate server at port 80 (http port)
 
String page = "";
String text = "";
double data;
 
 
void setup(){
sensors.begin(); // DS18B20  
//pinMode(A0, INPUT);
delay(1000);
Serial.begin(115200);
 
// Static IP Setup Info Here...
//WiFi.config(ip, gateway, subnet); 
 
WiFi.begin(ssid, password); //begin WiFi connection
Serial.println("");
 
// Wait for connection
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
 
Serial.println("");
Serial.print("Connected to ");
Serial.println(ssid);
Serial.print("IP address: ");
Serial.println(WiFi.localIP());
//lcd
lcd.init();
lcd.backlight();
lcd.setCursor(1, 0);
lcd.print(WiFi.localIP());
 
server.on("/data.txt", [](){
text = String(data,2)+"℃";
server.send(200, "text/html", text);
});
 
server.on("/", [](){
page = "<meta charset=\"UTF-8\"><h1>DS18B20数据采集ESP8266网页</h1><h1>Temperature:</h1> <h1 id=\"data\" align=\"center\">""</h1>\r\n";
page +="<style>h1{background:green;}</style>";
page += "<script>\r\n";
page += "var x = setInterval(function() {loadData(\"data.txt\",updateData)}, 1000);\r\n";
page += "function loadData(url, callback){\r\n";
page += "var xhttp = new XMLHttpRequest();\r\n";
page += "xhttp.onreadystatechange = function(){\r\n";
page += " if(this.readyState == 4 && this.status == 200){\r\n";
page += " callback.apply(xhttp);\r\n";
page += " }\r\n";
page += "};\r\n";
page += "xhttp.open(\"GET\", url, true);\r\n";
page += "xhttp.send();\r\n";
page += "}\r\n";
page += "function updateData(){\r\n";
page += " document.getElementById(\"data\").innerHTML = this.responseText;\r\n";
page += "}\r\n";
page += "</script>\r\n";
server.send(200, "text/html", page);
});
 
 server.begin();
 Serial.println("Web server started!");
}
 
 
void loop(){
  sensors.requestTemperatures(); // Send the command to get temperatures
  data = (sensors.getTempCByIndex(0));
  //data = analogRead(A0);
  delay(1500);
  server.handleClient();
    delay(500);
  lcd.backlight();
lcd.setCursor(1, 0);
lcd.print(WiFi.localIP());
lcd.setCursor(1, 1);
lcd.print("Temp:");
lcd.setCursor(6, 1);
lcd.print(String(data,2));
Serial.println(String(data,2));


}
