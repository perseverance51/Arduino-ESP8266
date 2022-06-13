#include <ESP8266WiFi.h>
#define AP_SSID "ESP8266AP" //这里改成你的AP名字--AP账号
#define AP_PSW "" //密码为空，也可以自行设置8位的密码
//配置IP信息
IPAddress local_IP(192,168,4,10);   //实例化IP地址
IPAddress gateway(192,168,4,1);     //实例化网关IP地址
IPAddress subnet(255,255,255,0);    //实例化子网掩码

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.print("配置AP网络信息...");
  WiFi.mode(WIFI_AP);   //设置WIFI模式为AP模式
  delay(2000);
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "网络信息成功" : "网络信息失败");  //配置AP网络信息---并且串口提示输出
  Serial.print("设置AP......");
  bool result = WiFi.softAP(AP_SSID, AP_PSW);  //设置账号和密码,通道为1，wifi不隐藏，最大连接数=4//返回值：是否成功
  if(result){   
  Serial.println("账号密码设置成功");
  Serial.println(String("WIFI_AP IP 地址是: ") + String(WiFi.softAPIP().toString()));
  //WiFi.softAPIP()    获取AP的IP地址--就是IPAddress local_IP(192,168,4,10)
  Serial.println(String("MAC 地址是： ") + WiFi.softAPmacAddress().c_str());
  //WiFi.softAPmacAddress().c_str()   获取AP的mac地址并转化成指针
  
}else{
  Serial.println("账号密码设置失败");
}
Serial.println("设置完成");
}

void loop() { 
//不断打印当前的station个数
int i=WiFi.softAPgetStationNum();
Serial.println(String("SAT连接数: ") +String(i));
delay(5000);
 }
