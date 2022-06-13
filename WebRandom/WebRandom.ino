#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Adafruit_NeoPixel.h>
#define PIN   4       //ws2812的控制脚
#define NUMPIXELS      7  //ws2812的灯珠数量，后面要通过循环遍历
#define ssid "ESP8266"//名称
#define pssd ""//密码为空
#define RANDOMTIME     5000  //随机变换颜色的间隔时间，毫秒
#define BREATHTIME     20    //呼吸效果的快慢频率,毫秒
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

ESP8266WebServer esp8266_server(80);
byte showRandom = 1;  //是否随机显示颜色
byte showFlash = 0;  //是否闪烁
byte showBreath = 1;  //是否呼吸
uint8_t rrr = 50;   //RGB的颜色
uint8_t ggg = 50;
uint8_t bbb = 50;
uint8_t bright = 60; //亮度
unsigned long lastRandomTime = 0;  //最后一次随机变换的时间
unsigned long lastBreathTime = 0;   //最后一次呼吸的时间
byte breathDirection=0;  //呼吸变换的方向，是变暗还变亮
void setup() {
  pixels.begin();
  Serial.begin(115200);
  pinMode(2, OUTPUT); //设置内置LED引脚为输出模式以便控制LED
  WiFi.mode(WIFI_AP); //设置成AP模式
  WiFi.softAP(ssid, pssd);
  delay(10);
  IPAddress ip = WiFi.softAPIP(); //获取ip地址
  Serial.println(ip);//打印出来ip地址
  esp8266_server.begin();                           // 启动网站服务
  esp8266_server.on("/", HTTP_GET, handleRoot);     // 设置服务器根目录即' / '的函数'handleRoot'
  esp8266_server.onNotFound(handleNotFound);        // 设置处理404情况的函数'handleNotFound'

  Serial.println("HTTP esp8266_server started");
}

void loop(void) {
  esp8266_server.handleClient();                     // 检查http服务器访问
  if (showRandom == 1) {
    doRandom();
  }
  if(showBreath==1){
    doBreath();
  }
  showLed();

}

void doBreath(){
  if(millis()-lastBreathTime > BREATHTIME){
    if(breathDirection == 0){//判断亮度是增加还是减少
      bright-=1;
      if(bright==0){
        breathDirection=1;
      }
    }
    if(breathDirection==1){
      bright+=1;
      if(bright==155){
        breathDirection=0;
      }
    }
    lastBreathTime=millis();
    //Serial.println(bright);
  }
}
void doRandom() {
  if (millis() - lastRandomTime > RANDOMTIME) {
    rrr = random(256);
    ggg = random(256);
    bbb = random(256);
    lastRandomTime = millis();
  }

}
void handleRoot() {

  //处理请求的参数,使用的是get方法，也许post会更好些，没有研究明白
  Serial.println("当前请求中请求体数量:"); 
  Serial.println(esp8266_server.args());
  if (esp8266_server.hasArg("random")) {
    esp8266_server.arg("random") == "true" ? showRandom = 1 : showRandom = 0;
  }
  if (esp8266_server.hasArg("flash")) {
    esp8266_server.arg("flash") == "true" ? showFlash = 1 : showFlash = 0;
  }
  if (esp8266_server.hasArg("breath")) {
    esp8266_server.arg("breath") == "true" ? showBreath = 1 : showBreath = 0;
  }
  if (esp8266_server.hasArg("bright")) {
    bright = String(esp8266_server.arg("bright")).toInt();
  }
  if (esp8266_server.hasArg("r")) {
    rrr = String(esp8266_server.arg("r")).toInt();
  }
  if (esp8266_server.hasArg("g")) {
    ggg = String(esp8266_server.arg("g")).toInt();
  }
  if (esp8266_server.hasArg("b")) {
    bbb = String(esp8266_server.arg("b")).toInt();
  }
  for (int i = 0; i < esp8266_server.args(); i++) {
    String t = esp8266_server.argName(i) + "=" + String(esp8266_server.arg(i));
    Serial.println(t);
  }
  //Serial.print("  bright=");
  //Serial.print(bright);
  //把当前的rgb数值更新到网页里
  String checkboxRandom = "";
  if (showRandom == 1) {
    checkboxRandom = "checked=\'true\'";
  }
  String checkboxFlash = "";
  showFlash ? checkboxFlash = "checked=\'true\'" : true;
  String checkboxBreath = "";
  showBreath ? checkboxBreath = "checked=\'true\'" : true;
  String myhtmlPage =
    String("") +
    "<!DOCTYPE html>" +
    "<html lang=\'zh-CN\'>" +
    "<head>" +
    "    <meta charset=\'UTF-8\'>" +
    "    <meta name=\'viewport\' content=\'width=device-width, initial-scale=1.4,  user-scalable=yes\' />" +
    "    <title>It\'s a model of html</title>" +
    "    <script>" +
    "        function postLed() {" +
    "            let checkRandom = document.getElementById(\'checkRandom\').checked;" +
    "            let checkBreath = document.getElementById(\'checkBreath\').checked;" +
    "            let checkFlash = document.getElementById(\'checkFlash\').checked;" +
    "            let bright = document.getElementById(\'rangeBright\').value;" +
    "            let rrr = document.getElementById(\'rangeR\').value;" +
    "            let ggg = document.getElementById(\'rangeG\').value;" +
    "            let bbb = document.getElementById(\'rangeB\').value;" +
    "            let args = window.location.href.split(\'?\')[0];" +
    "            args += \'random=\' + checkRandom + \'breath=\' + checkBreath + \'flash=\' + checkFlash;" +
    "            args += \'bright=\' + bright +  \'r=\' + rrr + \'g=\' + ggg + \'b=\' + bbb;" +
    "            console.log(args);" +
    "            window.location.href=args;" +
    "        }" +
    "    </script>" +
    "</head>" +
    "<body>" +
    "    </br>" +
    "    <input type=\'checkbox\' id=\'checkRandom\' " +
    checkboxRandom +
    "    ><span>随机</span></br>" +
    "    <input type=\'checkbox\' id=\'checkFlash\'" +
    checkboxFlash +
    "><span>闪烁</span></br>" +
    "    <input type=\'checkbox\' id=\'checkBreath\'" +
    checkboxBreath +
    "><span>呼吸</span></br>" +
    "    <span>亮度</span><input id=\'rangeBright\' type=\'range\' value=\'" +
    String(bright) +
    "\' min=\'0\' max=\'255\'></br>" +
    "    <span>红色</span><input id=\'rangeR\' type=\'range\' value=\'" +
    String(rrr) +
    "\' min=\'0\' max=\'255\'></br>" +
    "    <span>绿色</span><input id=\'rangeG\' type=\'range\' value=\'" +
    String(ggg) +
    "\' min=\'0\' max=\'255\'></br>" +
    "    <span>蓝色</span><input id=\'rangeB\' type=\'range\' value=\'" +
    String(bbb) +
    "\' min=\'0\' max=\'255\'></br>" +
    "    </br>" +
    "    <input type=\'button\' id=\'btn1\' οnclick=\'postLed()\' value=\'提交\' />" +
    "</body>" +
    "</html>";

  esp8266_server.send(200, "text/html", myhtmlPage);

}

// 设置处理404情况的函数'handleNotFound'
void handleNotFound() {
  esp8266_server.send(404, "text/plain", "404: Not found"); // 发送 HTTP 状态 404 (未找到页面) 并向浏览器发送文字 "404: Not found"
}
//遍历更新每一个灯珠
void showLed() {
  pixels.setBrightness(bright);
  pixels.clear();
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, rrr, ggg, bbb); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(1);
  }
}
