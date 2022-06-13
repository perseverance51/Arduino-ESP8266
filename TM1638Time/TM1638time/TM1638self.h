/*
数据命令字（见TM1638数据手册）
B7  B6  B5  B4  B3  B2  B1  B0    字节     说明
0    1   0   0   0   0   1   0     42     读键扫数据
0    1   0   0   0   0   0   0     40     自动地址增加显示模式
0    1   0   0   0   1   0   0     44     指定地址显示模式
寄存器地址命令：基地址为C0 ,偏移地址为00H - 0FH 具体见数据手册图2和7.2 地址命令设置
显示控制命令： 88H ,89H ,8AH,8BH,8CH,8DH,8EH,8FH分别对应显示亮度为十六分之一，二，四，十，十一，十二，十三，十四 。 80H显示关，
*/
//#ifndef TM1638self_h
//#define TM1638self_h

#include <Arduino.h>

class TM1638 {
    private:
        const byte DATA_COMMAND_WRITE_AUTO = 0x40;  //数据命令字：数据写到显示寄存器，自动地址增加。
        const byte DATA_COMMAND_WRITE_ADDR = 0x44; //数据命令字：数据写到固定地址
        const byte DATA_COMMAND_READ_KEY = 0x42;   //数据命令字：读键值
        const byte BASE_ADDR = 0XC0; //地址命令：设置地址到显示寄存器第一个地址0xC0。在此基础上的偏移地址为00H-0FH
        const byte DISP_COMMAND_HIGH_LIGHT = 0x8F; //显示控制：设开，并最亮
        const byte DISP_COMMAND_CLOSE = 0x80;       //关闭显示
        uint8_t CLK,DIO,STB;

    public:
        //亮度的八个级别
        unsigned char Light[8]={0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F};
         //共阴数码管显示代码 0，1，2，3，4，5，6，7，8，9，A,B,C,D,E,F。
        unsigned char tab[16]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};
        //共阴4位管，小时个位管的第8位即第b7位是秒的显示位。所以秒闪的程序要刷新小时个位管
         //例入，秒闪灯亮时，即B7位要置1，如原来0的显示码是3F，把B7位置1后就变成了B7,这样再刷新小时个位时秒灯就亮了
        unsigned char tab_second[16]={0xBF,0x86,0xDB,0xCF,0xE6,0xED,0xFD,0x87,0xFF,0xEF,0xF7,0xFC,0xB9,0xDE,0xF9,0xF1};
        TM1638(const int clk, const int dio , const int stb);  //构造函数，初始化显示
        void WriteCommand(unsigned char  command);  //向TM1638写入指令
        void WriteData(unsigned char data);   //向TM1638写入数据
        void WriteAddrData(unsigned char relative_addr, unsigned char data); //relative_addr相对于0xC0基地址的相对地址
        unsigned char ReadData(void);  //读取TM1638扫描键数据
        unsigned char ReadKey(void);   //直接返回键值
        bool SelfTest(void);           //TM1638及数码管自检函数
        void Disp8(const unsigned char message[]); //数码管显示刷新函数
        //void SecondBlink(int blink_flag);
        void SecondBlink(int blink_flag,int showtime5); //秒灯闪烁函数

};
