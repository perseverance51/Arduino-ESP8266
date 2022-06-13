/*  
 *  
 *  ESP32/ESP8266 example of downloading and uploading a file from or to the ESP device's Filing System
 *  仅支持ESP32
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

#ifdef ESP8266
#define SD_CS_pin           D2         // The pin on Wemos D1 Mini for SD Card Chip-Select
#else
#define SD_CS_pin           5        // Use pin 5 on MH-T Live ESP32 version of Wemos D1 Mini for SD Card Chip-Select
#endif  
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void setup(void){
  Serial.begin(115200);
  if (!WiFi.config(local_IP, gateway, subnet, dns)) { //WiFi.config(ip, gateway, subnet, dns1, dns2);
    Serial.println("WiFi STATION Failed to configure Correctly"); 
  } 
  wifiMulti.addAP("MERCURY_D268G", "pba5ayzk"); // 将需要连接的一系列WiFi ID和密码输入这里
  wifiMulti.addAP("CMCC-DyVv", "pba5ayzk"); // ESP8266-NodeMCU再启动后会扫描当前网络
  wifiMulti.addAP("MERCURY_2.4G_135E", "5a41t7ep"); // 环境查找是否有这里列出的WiFi ID。如果有

  
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
  if (!SD.begin(SD_CS_pin)) { // ESP32 CS uses D5;ESP8266  SD-Card CS uses D8 
    Serial.println(F("Card failed or not present, no SD Card data logging possible..."));
    SD_present = false; 
  } 
  else
  {
    Serial.println(F("Card initialised... file access enabled..."));
    SD_present = true; 
  }
  // 注意:使用ESP32和SD_Card读卡器需要1K到4.7K的电阻上拉到3v3的MISO线，否则它们不起作用。
  //----------------------------------------------------------------------   
  /*********************服务器命令**************************/
  
  server.on("/",         HomePage);//主页访问
  server.on("/download", File_Download);//下载页面访问
  server.on("/upload",   File_Upload);//上传页面访问
  //客户端通过upload页面,向服务器发送文件(请求方法POST),则回复状态码 200 给客户端,并且运行处理文件上传函数
  server.on("/fupload",  HTTP_POST,[](){ server.send(200);}, handleFileUpload);
  ///////////////////////////// End of Request commands
  server.begin();
  Serial.println("HTTP server started");
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void loop(void){
  server.handleClient(); // Listen for client connections
}

// All supporting functions from here...
/****************Http Server 主页************************/
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
  else SelectInput("","download","download");//Enter filename to download
}
/****************文件下载************************/
void SD_file_download(String filename){
  if (SD_present) { 
    File download = SD.open("/"+filename);
    if (download) {
      server.sendHeader("Content-Type", "text/text");
      server.sendHeader("Content-Disposition", "attachment; filename="+filename);
      server.sendHeader("Connection", "close");
      server.streamFile(download, "application/octet-stream");
      download.close();
    } else ReportFileNotPresent("download"); 
  } else ReportSDNotPresent();
}
/****************获取文件列表************************/
String listDir(fs::FS &fs, const char * dirname, uint8_t levels){
  String str =F("<li>File List: ");
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if(!root){
    Serial.println("Failed to open directory");
    return str;
  }
  if(!root.isDirectory()){
    Serial.println("Not a directory");
    return str;
  }

  File file = root.openNextFile();
  while(file){
    if(file.isDirectory()){
      Serial.print("DIR : ");
      Serial.println(file.name());
      str  += file.name();
       str  += F("</li><br>");
      str +="<li>File List: ";
      if(levels){
        listDir(fs, file.name(), levels -1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
      str += file.name();
     str += F("  SIZE: ");
    str +=file.size();
    str +=F("</li><br>");
    str +=F("<li>File List: ");
    }
    file = root.openNextFile();
  }
   return str;
}
/******* Http网页端 上传文件页面*******/
void File_Upload(){
  append_page_header();
  webpage += F("<h3>请选择文件上传</h3>"); 
  webpage += F("<FORM action='/fupload' method='post' enctype='multipart/form-data'>");
  webpage += F("<input class='buttons' style='width:40%' type='file' name='fupload' id = 'fupload' value=''><br>");
  webpage += F("<br><button class='buttons' style='width:10%' type='submit'>上传文件</button><br>");
  webpage += F("<a href='/'>[ 返回 ]</a><br><br>");
  append_page_footer();
  server.send(200, "text/html",webpage);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
File UploadFile; 
void handleFileUpload(){ // upload a new file to the Filing system
  String filelist="";
  HTTPUpload& uploadfile = server.upload(); // See https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer/srcv
                                            // For further information on 'status' structure, there are other reasons such as a failed transfer that could be used
  if(uploadfile.status == UPLOAD_FILE_START)
  {
    String filename = uploadfile.filename;
    if(!filename.startsWith("/")) filename = "/"+filename;// 为上传文件名前加上"/"
    Serial.print("Upload File Name: "); Serial.println(filename);// 通过串口监视器输出上传文件的名称
    SD.remove(filename);                         //删除以前的版本，否则将再次附加数据文件
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
      filelist = listDir(SD, "/", 0);//获取SD卡文件列表
      webpage = "";
      append_page_header();
      webpage += F("<h3>文件上传成功！</h3>"); 
      webpage += F("<h2>Uploaded File Name: "); webpage += uploadfile.filename+"</h2>"; //上传完文件，显示文件名称
      webpage += F("<h2>File Size: "); webpage += file_size(uploadfile.totalSize) + "</h2><br>"; //上传完文件，显示文件大小
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
  server.client().stop(); // 因为没有发送内容长度，所以需要停止
}
/********************Http网页端下载页面*******************/
void SelectInput(String heading1, String command, String arg_calling_name){
  SendHTML_Header();
  webpage += F("<h3>"); webpage += heading1 + "</h3>";
  webpage += F("<h3>请输入文件名后按Enter键或点击下载按钮进行下载");  
  webpage += "</h3>";
  webpage += F("<FORM action='/"); webpage += command + "' method='post'>"; // Must match the calling argument e.g. '/chart' calls '/chart' after selection but with arguments!
  webpage += F("<input type='text' name='"); webpage += arg_calling_name; webpage += F("' value=''>");//拼接文件名开始

  webpage +=F("<input type='submit' value='下载'></form><br>");//下载按钮
 
  webpage += F("<type='submit' name='"); webpage += arg_calling_name; webpage += F("' value='下载'><br>");//拼接文件名结束
  webpage += F("<a href='/'>[返回]</a>");//'/'返回首页
  append_page_footer();
  SendHTML_Content();
  SendHTML_Stop();
}
//当没有SD设备时，响应信息
void ReportSDNotPresent(){
  SendHTML_Header();
  webpage += F("<h3>No SD Card present</h3>"); 
  webpage += F("<a href='/'>[返回]</a><br><br>");
  append_page_footer();
  SendHTML_Content();
  SendHTML_Stop();
}
//没有目标文件时的响应信息
void ReportFileNotPresent(String target){
  SendHTML_Header();
  webpage += F("<h3>File does not exist</h3>"); 
  webpage += F("<a href='/"); webpage += target + "'>[返回]</a><br><br>";
  append_page_footer();
  SendHTML_Content();
  SendHTML_Stop();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void ReportCouldNotCreateFile(String target){
  SendHTML_Header();
  webpage += F("<h3>Could Not Create Uploaded File (write-protected?)</h3>"); 
  webpage += F("<a href='/"); webpage += target + "'>[返回]</a><br><br>";
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
