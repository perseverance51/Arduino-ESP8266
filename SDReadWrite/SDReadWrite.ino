/*
  SD card read/write
 
 This example shows how to read and write data to and from an SD card file   
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4
   
 */
#include <SPI.h> 
#include <SD.h>
const int chipSelect = D8;//gpio 15
File myFile;

void setup()
{
  Serial.begin(9600);
  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
   pinMode(2, OUTPUT);
   
  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  
//打开文件。注意，一次只能打开一个文件，
//所以你必须在打开另一个之前关闭这个。
  myFile = SD.open("write.txt", FILE_WRITE);
  
//如果文件打开了，写入:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("perseverance9527");
  // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
   //如果文件没有打开，打印一个错误:
    Serial.println("error opening test.txt");
  }
}  


void loop()
{
  delay(5000);
    //重新打开文件读取:
  myFile = SD.open("write.txt");//创建
  if (myFile) {
    Serial.println("write.txt:");
    
    //读取文件，直到文件中没有其他内容:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    //关闭文件:
    myFile.close();
  } else {
    //如果文件没有打开，打印一个错误:
    Serial.println("error opening test.txt");
  }

}
