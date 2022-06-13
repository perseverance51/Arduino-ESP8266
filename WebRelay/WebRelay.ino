
#include "ESP8266WiFi.h"
#include "ESPAsyncWebServer.h"

#define RELAY_NO    false  //继电器默认状态

#define NUM_RELAYS  4    //4路继电器

byte relayGPIOs[NUM_RELAYS] = {D0, D1, D2, D3};

const char* ssid =  "MERCURY_D268G";                     //wifi名称
const char* password ="pba5ayzk";              //wifi密码
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
<meta charset='UTF-8'>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 3.0rem;}
    p {font-size: 3.0rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 34px}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 68px}
    input:checked+.slider {background-color: #2196F3}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
  </style>
</head>
<body>
  <h2>ESP8266网页控制页面</h2>
  %BUTTONPLACEHOLDER%
<script>function toggleCheckbox(element) {
  var xhr = new XMLHttpRequest();
  if(element.checked){ xhr.open("GET", "/update?relay="+element.id+"&state=1", true); }
  else { xhr.open("GET", "/update?relay="+element.id+"&state=0", true); }
  xhr.send();
}</script>
</body>
</html>
)rawliteral";

// Replaces placeholder with button section in your web page
String processor(const String& var){
  //Serial.println(var);
  if(var == "BUTTONPLACEHOLDER"){
    String buttons ="";
    for(int i=0; i<NUM_RELAYS; i++){
      String relayStateValue = relayState(i);
      buttons+= "<h4>继电器 #" + String(i+1) + " - GPIO " + relayGPIOs[i] + "</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"" + String(i) + "\" "+ relayStateValue +"><span class=\"slider\"></span></label>";
    }
    return buttons;
  }
  return String();
}

String relayState(int numRelay){
  if(RELAY_NO){
    if(digitalRead(relayGPIOs[numRelay])){
      return "";
    }
    else {
      return "checked";
    }
  }
  else {
    if(digitalRead(relayGPIOs[numRelay])){
      return "checked";
    }
    else {
      return "";
    }
  }
  return "";
}

void setup(){
  Serial.begin(115200);

  for(int i=0; i<NUM_RELAYS; i++){
    pinMode(relayGPIOs[i], OUTPUT);
    if(RELAY_NO){
      digitalWrite(relayGPIOs[i], HIGH);//开启继电器
    }
    else{
      digitalWrite(relayGPIOs[i], LOW);//关闭继电器
    }
  }
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  //更新继电器状态页面
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    String inputMessage2;
    String inputParam2;
  //获取异步js发起的get请求 relay
    if (request->hasParam("relay") & request->hasParam("state")) {
      inputMessage = request->getParam("relay")->value();
      inputParam = "relay";
      inputMessage2 = request->getParam("state")->value();
      inputParam2 = "state";
      if(RELAY_NO){
        Serial.print("NO ");
        digitalWrite(relayGPIOs[inputMessage.toInt()], !inputMessage2.toInt());
      }
      else{
        Serial.print("NC ");
        digitalWrite(relayGPIOs[inputMessage.toInt()], inputMessage2.toInt());
      }
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.println(inputMessage + inputMessage2);
    request->send(200, "text/plain", "OK");
  });
  server.begin();
}
  
void loop() {

}
      
