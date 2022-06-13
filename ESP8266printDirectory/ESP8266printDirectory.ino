/*对Micro SD卡内根目录内容进行打印
 * 
 */
#include <SPI.h>
#include <SD.h>

// 创建File实例化对象
File myFile;

const int chipSelect = D8;//gpio 15

void setup()
{
    // 初始化硬件串口并设置波特率为9600
    Serial.begin(9600);
    while (!Serial)
    {
        ; //等待串口打开
    }

    Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect)) 
    {
        Serial.println("initialization failed!");
        return;
    }
    Serial.println("initialization done.");

    //打开文件。 请注意，一次只能打开一个文件
    //您必须先关闭该文件再打开另一个文件。
    myFile = SD.open("/");

    printDirectory(myFile, 0);

    Serial.println("done!");
}

void loop()
{
}

void printDirectory(File dir, int numTabs)
{
    while (true)
    {

        File entry = dir.openNextFile();
        if (!entry)
        {
            // 如果没有文件则跳出循环
            break;
        }
        for (uint8_t i = 0; i < numTabs; i++)
        {
            Serial.print('\t');
        }
        Serial.print(entry.name());
        if (entry.isDirectory())
        {
            Serial.println("/");
            printDirectory(entry, numTabs + 1);//递归
        }
        else
        {
            // 文件会大小,文件夹则不打印大小
            Serial.print("\t\t");
            Serial.println(entry.size(), DEC);
        }
        entry.close();
    }
}
