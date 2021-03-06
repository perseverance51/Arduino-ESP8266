/*  
 *  
 *  ESP32/ESP8266 example of downloading and uploading a file from or to the ESP device's Filing System
*/
#ifdef ESP8266
  #include <ESP8266WiFi.h>       // Built-in
  #include <ESP8266WiFiMulti.h>  // Built-in
  #include <ESP8266WebServer.h>  // Built-in
  #include <ESP8266mDNS.h>
#else
  #include <WiFi.h>              // Built-in
  #include <WiFiMulti.h>         // Built-in
  #include <ESP32WebServer.h>    // https://github.com/Pedroalbuquerque/ESP32WebServer download and place in your Libraries folder
  #include <ESPmDNS.h>
  #include "FS.h"
#endif

#include "Network.h"
#include "Sys_Variables.h"
#include "CSS.h"
#include <SD.h> 
#include <SPI.h>

#ifdef ESP8266
  ESP8266WiFiMulti wifiMulti; 
  ESP8266WebServer server(80);
#else
  WiFiMulti wifiMulti;
  ESP32WebServer server(80);
#endif

//温湿度传感器
#include "DHT.h"

// 获取时间
#include <time.h> 
#define TZ 8            // 中国时区为8
#define DST_MN 0        // 默认为0
#define TZ_MN ((TZ)*60) //时间换算
#define TZ_SEC ((TZ)*3600)
#define DST_SEC ((DST_MN)*60)
time_t now;


#define DHTPIN D4   // used for ESP8266
#define DHTYPE DHT11
DHT dht(DHTPIN, DHTYPE);

File UploadFile; 
bool ledState = LOW;
const long interval = 6000;//设置延时时间间隔
unsigned long previousMillis = 0;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void setup(void){
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);//板载led灯作为指示
  if (!WiFi.config(local_IP, gateway, subnet, dns)) { //WiFi.config(ip, gateway, subnet, dns1, dns2);
    Serial.println("WiFi STATION Failed to configure Correctly"); 
  } 
  wifiMulti.addAP("MERCURY_D268G", "pba5ayzk"); // 将需要连接的一系列WiFi ID和密码输入这里
  wifiMulti.addAP("CMCC-DyVv", "pba5ayzk"); // ESP8266-NodeMCU再启动后会扫描当前网络
  wifiMulti.addAP("MERCURY_2.4G_135E", "5a41t7ep"); // 环境查找是否有这里列出的WiFi ID。如果有
  //wifiMulti.addAP(ssid_4, password_4);  // You don't need 4 entries, this is for example!
  
  Serial.println("Connecting ...");
  while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(250); Serial.print('.');
  }
  Serial.println("\nConnected to "+WiFi.SSID()+" Use IP address: "+WiFi.localIP().toString()); // Report which SSID and IP is in use
  // The logical name http://fileserver.local will also access the device if you have 'Bonjour' running or your system supports multicast dns
  if (!MDNS.begin(servername)) {          // Set your preferred server name, if you use "myserver" the address would be http://myserver.local/
    Serial.println(F("Error setting up MDNS responder!")); 
    ESP.restart(); 
  } 
  #ifdef ESP32
    // Note: SD_Card readers on the ESP32 will NOT work unless there is a pull-up on MISO, either do this or wire one on (1K to 4K7)
    Serial.println(MISO);
    pinMode(2,INPUT_PULLUP);
  #endif
  Serial.print(F("Initializing SD card...")); 
  if (!SD.begin(SD_CS_pin)) { // see if the card is present and can be initialised. Wemos SD-Card CS uses D8 
    Serial.println(F("Card failed or not present, no SD Card data logging possible..."));
    SD_present = false; 
  } 
  else
  {
    Serial.println(F("Card initialised... file access enabled..."));
    SD_present = true; 
  }
  // Note: Using the ESP32 and SD_Card readers requires a 1K to 4K7 pull-up to 3v3 on the MISO line, otherwise they do-not function.
  //----------------------------------------------------------------------   
  ///////////////////////////// Server Commands 
  server.on("/",         HomePage);//主页面
  server.on("/download", File_Download);//下载页面
  server.on("/upload",   File_Upload);//上传页面
  server.on("/fupload",  HTTP_POST,[](){ server.send(200);}, handleFileUpload);
  server.on("/delete",  File_Delete);//删除文件页面
  ///////////////////////////// End of Request commands
  server.begin();
  Serial.println("HTTP server started");
  configTime(TZ_SEC, DST_SEC, "ntp.ntsc.ac.cn", "ntp1.aliyun.com", "1.cn.pool.ntp.org"); //ntp获取时间，你也可用其他"pool.ntp.org","0.cn.pool.ntp.org","1.cn.pool.ntp.org","ntp1.aliyun.com"

   dht.begin();//初始化DHT11
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void loop(void){
  server.handleClient(); // Listen for client connections
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
     ledState = !ledState;//状态翻转
     digitalWrite(LED_BUILTIN, ledState);
     DHTdata();
     previousMillis = currentMillis;
}
}
// All supporting functions from here...
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void HomePage(){
  SendHTML_Header();
  webpage += F("<a href='/download'><button>Download</button></a>");
  webpage += F("<a href='/upload'><button>Upload</button></a>");
  append_page_footer();
  SendHTML_Content();
  SendHTML_Stop(); // Stop is needed because no content length was sent
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void File_Download(){ // This gets called twice, the first pass selects the input, the second pass then processes the command line arguments
  if (server.args() > 0 ) { // Arguments were received
    if (server.hasArg("download")) SD_file_download(server.arg(0));
  }
  else SelectInput("","download","download");
}

//获取网页端删除文件信息
void File_Delete(){ // This gets called twice, the first pass selects the input, the second pass then processes the command line arguments
  
  if (server.args() > 0 ) { // 参数名Arguments were received
    if (server.hasArg("FileName")) 
    SD_file_delete(server.arg("FileName"));//执行删除任务
  }
  else SelectInputFileName("警告：以下操作将执行的是删除SD文件内容的操作！","delete","FileName");//跳到删除界面
}

/****************服务端执行相应的删除文件************************/
void SD_file_delete(String filename){
  String Filename ="/";
    bool flag = false;
  Filename += filename;
 if (SD_present) { 
    File DeleteFile = SD.open("/"+filename);
 if (SD.exists(Filename)) {//判断文件是否存在
    Serial.println(filename +" is File exists");
    Serial.println(filename +" is Removing..." );
    // delete the file:
   flag = SD.remove(Filename);
  } else {
    Serial.println(filename +" File does not exist.");//文件不存在
  }  
 if (flag) {
      ReportFileDelete(filename);
//      server.sendHeader("Content-Type", "text/text");
//      server.sendHeader("Content-Disposition", "attachment; filename="+filename);
//      server.sendHeader("Connection", "close");
      //server.streamFile(DeleteFile, "application/octet-stream");通过该方法就可以把文件发送给客户端
    //  DeleteFile.close();
    } else ReportFileNotPresent(Filename); //没有目标文件时的响应信息
    DeleteFile.close();
  } else ReportSDNotPresent(); 
}
/****************服务端执行相应客户端文件下载请求内容************************/
void SD_file_download(String filename){
  if (SD_present) { //判断SD卡初始化成功
    File download = SD.open("/"+filename);
    if (download) {
      server.sendHeader("Content-Type", "text/text");
      server.sendHeader("Content-Disposition", "attachment; filename="+filename);
      server.sendHeader("Connection", "close");
      server.streamFile(download, "application/octet-stream");//通过该方法就可以把文件发送给客户端
      download.close();//关闭文件
    } else ReportFileNotPresent("download"); 
  } else ReportSDNotPresent();
}
/******* Http网页端 上传文件页面*******/
void File_Upload(){
  append_page_header();
  webpage += F("<h3>请选择上传文件</h3>"); 
  webpage += F("<FORM action='/fupload' method='post' enctype='multipart/form-data'>");
  webpage += F("<input class='buttons' style='width:40%' type='file' name='fupload' id = 'fupload' value=''><br>");
  webpage += F("<br><button class='buttons' style='width:10%' type='submit'>文件上传</button><br><br>");
  webpage += F("<a href='/'>[返回]</a><br><br>");
  append_page_footer();
  server.send(200, "text/html",webpage);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void handleFileUpload(){ // upload a new file to the Filing system
  HTTPUpload& uploadfile = server.upload(); // See https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer/srcv
                                            // For further information on 'status' structure, there are other reasons such as a failed transfer that could be used
  if(uploadfile.status == UPLOAD_FILE_START)
  {
    String filename = uploadfile.filename;
    if(!filename.startsWith("/")) filename = "/"+filename;
    Serial.print("Upload File Name: "); Serial.println(filename);
    SD.remove(filename);                         // Remove a previous version, otherwise data is appended the file again
    UploadFile = SD.open(filename, FILE_WRITE);  // Open the file for writing in SPIFFS (create it, if doesn't exist)
    filename = String();
  }
  else if (uploadfile.status == UPLOAD_FILE_WRITE)
  {
    if(UploadFile) UploadFile.write(uploadfile.buf, uploadfile.currentSize); // Write the received bytes to the file
  } 
  else if (uploadfile.status == UPLOAD_FILE_END)
  {
    if(UploadFile)          // If the file was successfully created
    {                                    
      UploadFile.close();   // Close the file again
      Serial.print("Upload Size: "); Serial.println(uploadfile.totalSize);
      
    UploadFile  = SD.open("/");
    String filelist = listDir(UploadFile, 0);//获取SD卡根目录文件列表
    
      webpage = "";
      append_page_header();
      webpage += F("<h3>文件上传成功!</h3>"); 
      webpage += F("<h2>Uploaded File Name: "); webpage += uploadfile.filename+"</h2>";
      webpage += F("<h2>File Size: "); webpage += file_size(uploadfile.totalSize) + "</h2><br>"; 
      
      webpage += F("<ul>");
      webpage +=  filelist + "</li><br>"; 
      webpage += F("</ul>");
      
      append_page_footer();
      server.send(200,"text/html",webpage);
    } 
    else
    {
      ReportCouldNotCreateFile("upload");
    }
  }
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void SendHTML_Header(){
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate"); 
  server.sendHeader("Pragma", "no-cache"); 
  server.sendHeader("Expires", "-1"); 
  server.setContentLength(CONTENT_LENGTH_UNKNOWN); 
  server.send(200, "text/html", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves. 
  append_page_header();
  server.sendContent(webpage);
  webpage = "";
}
//发送客户端请求页面
void SendHTML_Content(){
  server.sendContent(webpage);
  webpage = "";
}
//停止客户端请求
void SendHTML_Stop(){
  server.sendContent("");
  server.client().stop(); // Stop is needed because no content length was sent
}
/********************Http网页端下载页面*******************/
void SelectInput(String heading1, String command, String arg_calling_name){
  SendHTML_Header();
  webpage += F("<h3>"); webpage += heading1 + "</h3>"; 
  webpage += F("<h3>请输入文件名后按Enter键或点击下载按钮进行下载");  
    webpage += "</h3><br>";
  webpage += F("<form action='/"); webpage += command + "' method='post'>"; // Must match the calling argument e.g. '/chart' calls '/chart' after selection but with arguments!
  webpage += F("<input type='text' name='"); webpage += arg_calling_name; webpage += F("' value=''>");
  webpage += F("<type='submit' name='"); webpage += arg_calling_name; webpage += F("' value=''>&emsp;&emsp;");//拼接文件名开始
  webpage +=F("<input type='submit' value='下载'></form><br>");//下载按钮
  webpage += F("<type='submit' name='"); webpage += arg_calling_name; webpage += F("' value='下载'><br>");//拼接文件名结束
  webpage += F("<a href='/'>[ 返回 ]</a>");//'/'返回首页
  append_page_footer();
  SendHTML_Content();
  SendHTML_Stop();
}

/********************Http网页端删除页面*******************/
void SelectInputFileName(String heading1, String command, String arg_calling_name){
  SendHTML_Header();
  webpage += F("<h3>"); webpage += heading1 + "</h3>"; 
  webpage += F("<h3>请输入文件名后按Enter键或点击删除按钮进行删除");  
    webpage += "</h3><br>";
    //必须匹配调用参数，例如。'/chart'在选择后调用'/chart'，但带有参数!
  webpage += F("<form action='/"); webpage += command + "' method='get'>"; // Must match the calling argument e.g. '/chart' calls '/chart' after selection but with arguments!
  webpage += F("<input type='text' name='"); webpage += arg_calling_name; webpage += F("' value=''>");
  webpage += F("<type='submit' name='"); webpage += arg_calling_name; webpage += F("' value=''>&emsp;&emsp;");//拼接文件名开始
  webpage +=F("<input type='submit' value='删除'></form><br>");//删除按钮
  webpage += F("<type='submit' name='"); webpage += arg_calling_name; webpage += F("' value=''><br>");//拼接文件名结束
  webpage += F("<a href='/'>[ 返回 ]</a>");//'/'返回首页
  append_page_footer();
  SendHTML_Content();
  SendHTML_Stop();
}

//当没有SD设备时，响应信息
void ReportSDNotPresent(){
  SendHTML_Header();
  webpage += F("<h3>No SD Card present</h3>"); 
  webpage += F("<a href='/'>[ 返回 ]</a><br><br>");
  append_page_footer();
  SendHTML_Content();
  SendHTML_Stop();
}
//没有目标文件时的响应信息
void ReportFileNotPresent(String target){
  SendHTML_Header();
  webpage += F("<h3>目标文件不存在</h3>"); 
  webpage += F("<a href='/"); webpage += target + "'>[ 返回 ]</a><br><br>";
  append_page_footer();
  SendHTML_Content();
  SendHTML_Stop();
}
//目标文件删除成功后的响应信息
void ReportFileDelete(String target){
  SendHTML_Header();
  webpage += F("<br><h2>"); 
  webpage += target + "</h2>";
  webpage += F("<h2> 文件删除成功！</h2>"); 
  webpage += F("<a href='/'>[ 返回 ]</a>");//'/'返回首页
  append_page_footer();
  SendHTML_Content();
  SendHTML_Stop();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void ReportCouldNotCreateFile(String target){
  SendHTML_Header();
  webpage += F("<h3>Could Not Create Uploaded File (write-protected?)</h3>"); 
  webpage += F("<a href='/"); webpage += target + "'>[ 返回 ]</a><br><br>";
  append_page_footer();
  SendHTML_Content();
  SendHTML_Stop();
}
//计算上传的文件大小
String file_size(int bytes){
  String fsize = "";
  if (bytes < 1024)                 fsize = String(bytes)+" B";
  else if(bytes < (1024*1024))      fsize = String(bytes/1024.0,3)+" KB";
  else if(bytes < (1024*1024*1024)) fsize = String(bytes/1024.0/1024.0,3)+" MB";
  else                              fsize = String(bytes/1024.0/1024.0/1024.0,3)+" GB";
  return fsize;
}
//读取SD卡根目录下的文件
String listDir(File dir, int numTabs)
{
  String str =F("<li>File List:</li><br> <li>File name:");
    while (true)
    { 
        File entry = dir.openNextFile();
        if (!entry)
        {
            // 如果没有文件则跳出循环
                Serial.println("Failed to open directory");
            break;
        }
        for (uint8_t i = 0; i < numTabs; i++)
        {          
            Serial.print('\t');
        }
        Serial.print(entry.name());
        str  += entry.name();
        if (entry.isDirectory())
        {
           str  += F("/");
            Serial.println("/");
        str  += listDir(entry, numTabs + 1);//递归
        }
        else
        {
            // 文件会打印大小,文件夹则不打印大小
            str  += F("  size: ");
            Serial.print("\t\t");
            str  += entry.size();
            str +=F("</li><br>");//一个文件一行显示
            str +=F("<li>File name: ");//另一行开始
            Serial.println(entry.size(), DEC);
        }
        entry.close();
    }
     return str;
}

void DHTdata(){
    String dataString = "";
      float hum = dht.readHumidity();
      float t = dht.readTemperature();
      
    dataString +=NowTime();//获取时间
    dataString += "  Temp:";
    dataString += String(t);
    dataString += "℃  RH:"; 
    dataString += String(hum);
    dataString += "%"; 
        
File dataFile = SD.open("datalog.txt", FILE_WRITE);

  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    Serial.println(dataString);
      }  
  else {
    Serial.println("error opening datalog.txt");
  }
}

String NowTime()
{ 
  String str="";
  now = time(nullptr); //实例化时间
// struct tm *timeInfo;
  tm *timeInfo;//创建结构体指针变量，同上
  timeInfo = localtime(&now);
  char buff[20];
  sprintf_P(buff, PSTR("%04d-%02d-%02d %d : %d : %d"), timeInfo->tm_year + 1900, timeInfo->tm_mon + 1, timeInfo->tm_mday,timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
 // Serial.println(buff);//2021-11-29 21:41:52
  str = buff;
  return str;

}
