#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>//https://hub.fastgit.org/me-no-dev/ESPAsyncWebServer

//开启异步服务器80端口
AsyncWebServer server(80);

//配置wifi和密码
const char* ssid = "MERCURY_D268G";
const char* password = "pba5ayzk";
//表单html页面
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
<title>ESP 数据表单输入</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<meta charset="UTF-8">
</head><body>
<form action="/get">
输入1: <input type="text" name="input1">
<input type="submit" value="提交">
</form><br>
<form action="/get">
输入2: <input type="text" name="input2">
<input type="submit" value="提交">
</form><br>
<form action="/get">
输入3: <input type="text" name="input3">
<input type="submit" value="提交">
</form>
</body></html>)rawliteral";

//404页面
void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void setup() {
  Serial.begin(115200);
  //连接wifi
  WiFi.mode(WIFI_STA);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  //默认页面
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });
  
  //表单跳转页面
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    // 获取 input1 提交的数据 
    if (request->hasParam("input1")) {
      inputMessage = request->getParam("input1")->value();
      inputParam = "input1";
    }
    // 获取 input2 提交的数据 
    else if (request->hasParam("input2")) {
      inputMessage = request->getParam("input2")->value();
      inputParam = "input2";
    }
    // 获取 input3 提交的数据 
    else if (request->hasParam("input3")) {
      inputMessage = request->getParam("input3")->value();
      inputParam = "input3";
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.println(inputMessage);
    //跳转页面显示
    request->send(200, "text/html", "HTTP GET request sent to your ESP on input field ("
    + inputParam + ") with value: " + inputMessage +
    "<br><a href=\"/\">Return to Home Page</a>");
  });
  server.onNotFound(notFound);
  server.begin();
}

void loop() {
  
}
