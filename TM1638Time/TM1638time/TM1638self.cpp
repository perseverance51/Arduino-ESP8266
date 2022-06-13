#include "TM1638self.h"

//最基础的写数据函数。从每个data字节数的最低位B0写起，最后是B7位.由DIO脚输出数据时序。由每个CLK时钟完成有效输出。
void TM1638::WriteData(unsigned char data){    
    unsigned char i;
    pinMode(DIO,OUTPUT);
    for (i=0;i<8;i++){
        digitalWrite(CLK,LOW);     //时钟置低
        if (data&0x01){             //判断data传送位是0或1
            digitalWrite(DIO,HIGH);  //如果是1，转化成DIO脚输出高电平
        }else {
            digitalWrite(DIO,LOW);    //如果是0，转化成DIO脚输出低电平
        }
        data>>=1;
        digitalWrite(CLK,HIGH);  //时钟置高，，完成一个bit的传送
    }
}

//向TM1638传送命令字，命令字本身也是按数据传送，只是用STB脚的一次拉低拉高做为命令字的区分。
void TM1638::WriteCommand(unsigned char command){
    digitalWrite(STB,LOW );
    WriteData(command);
    digitalWrite(STB,HIGH);  //完成一次命令传送
}


//显示数据写入指定地址的寄存器。在一个STB的低到高时序中，先传送的数据为寄存器地址，再传送需写入该寄存器的显示数据
void TM1638::WriteAddrData(unsigned char relative_addr, unsigned char data){
    WriteCommand(DATA_COMMAND_WRITE_ADDR);
    digitalWrite(STB,LOW);    //=======开始传送
    WriteData(BASE_ADDR|relative_addr);   //指定地址模式下，发送地址0xc0|相对地址。即基地址C0H+偏移量relative_addr
    WriteData(data);
    digitalWrite(STB,HIGH);  //===============因为固定地址模式下每次发送完地址和数据需要一个STB的上升沿区隔

};

//最基础的读数据函数，以时序模拟的方式实现。每次读到的是数据字节的最低位B0位，所以连续读时bit位要右移。
 unsigned char TM1638::ReadData(void){
    unsigned char i,result=0;
    pinMode(DIO,INPUT);

    for (i=0;i<8;i++){   
        result>>=1;            //先压入一个0bit,由于是读到低位这节，所以要右移
        digitalWrite(CLK,LOW);   //CLK 拉低
        if (digitalRead(DIO)==HIGH)  //如果读到的是高电平
            result|=0x80;           //高电平，则把当前bit置一，否则默认置0.
        digitalWrite(CLK,HIGH);     //完成一个bit的读取
    }
    return result;
 };

//键值读取函数
 unsigned char TM1638::ReadKey(void){
    unsigned char c[4],i,key_value=0;
        digitalWrite(STB,LOW);   //=====================
  WriteData(DATA_COMMAND_READ_KEY);              //发送读键数据 命令
  for(i=0;i<4;i++){             //开始读键值
        c[i]=ReadData();       //一次读八位，共读四个字节
    }
        digitalWrite(STB,HIGH);     //==================              
//4个字节读键数据的每个bit含义如下。由于本程序电路只有8个键，都连接在K3脚上，因此只有8个键值
//程序内思路最终将四个字节合并成一个字节。，方便后继处理。
//  B7   B6[K1]   B5[k2]   B4[k3]   B3   B2[k1]   B1[k2]   B0[k3]   对应存储数组
//        ks2      ks2      ks2           ks1      ks1      ks1      c[0]
//        ks4      ks4      ks4           ks3      ks3      ks3      c[1]
//        ks6      ks6      ks6           ks5      ks5      ks5      c[2]
//        ks8      ks8      ks8           ks7      ks7      ks7      c[3]
//连接K3的八键按下后，再通过移位相加后就变成一个字节/
//  ks8   ks6      ks4      ks2     ks7    ks5     ks3      ks1
//由于在硬件上电路板设计成如下的对应关系///
//  键8    键7      键6      键5     键4     键3     键2      键1
//所以最终下面 if((0x01<<i)==key_value)比较下来以的，i值就是第几个键值被按到
  for(i=0;i<4;i++){
        key_value|=c[i]<<i;  //连接K3的八键按下后，再通过移位相加后就变成一个字节如下/
                             //  B7    B6       B5       B4      B3    B2       B1       B0
                             //  ks8   ks6      ks4      ks2     ks7    ks5     ks3      ks1
    } 

    for(i=0;i<8;i++) {
        if((0x01<<i)==key_value)   //第i位键值位Bit为1，则取得键值为i+1
        break;
    }
  return i+1;   //键值为1，2，，，8

};

//对象初始化
TM1638::TM1638(const int clk, const int dio , const int stb){
    unsigned char i;
    CLK=clk;
    DIO=dio;
    STB=stb;
    pinMode(clk,OUTPUT);
    pinMode(dio,OUTPUT);
    pinMode(stb,OUTPUT);
    //显示一次
    WriteCommand(DISP_COMMAND_HIGH_LIGHT);//亮度最亮
    WriteCommand(DATA_COMMAND_WRITE_AUTO); //采用自动地址加1显示模式
    digitalWrite(STB,LOW); //写地址和写数据时STB要保持 LOW
    WriteData(BASE_ADDR);   //设置基地址
    for (i=0;i<16;i++){
        WriteData(0x00);
    }
    digitalWrite(STB,HIGH);      
};

//自检程序
bool TM1638:: SelfTest(void){           //自增地址显示模式
    for (int j=0;j<16;j++){
        if (j<8){                     //调亮度控制。先暗到亮再亮到暗
            WriteCommand(Light[j]);
        }else{ 
            WriteCommand(Light[15-j]);
        }
        WriteCommand(DATA_COMMAND_WRITE_AUTO);  //自增地址显示模式
        digitalWrite(STB,LOW);  //=============stb low 
        WriteData(BASE_ADDR);    
        for (int i=0;i<16;i++){   //显示0123456789ABCDEF
            WriteData(tab[j]);
        }
        digitalWrite(STB,HIGH);   //==============HIGH
        delay(300);
    }
};

//显示电路上的月日时分秒温度
//unsigned char message[10]={0,0,0,0,0,0,0,0,0,0};   该结构在主程序里定义
void TM1638::Disp8(const unsigned char message[]){    //固定地址显示模式
    unsigned char  t1,t2,t3;
    //显示时间，message的前八位
    for (int i=0;i<8;i++){       
        WriteAddrData(i*2,tab[message[i]]);
    }
    //显示温度，message的第九位和第十位
    t1=tab[message[8]];
    t2=tab[message[9]];
    for (int j=0;j<8;j++){  //对应写入显示寄存器的大循环
        t3=0;
         //处理把行向表示的字符处理成列向显示的字符
        switch (t1&0x01){
            case 0:
                if (t2&0x01==1){
                    t3=t3|0x01; //先把t2的最低位压入b1位，对应SEG10位
                    t3=t3<<1;  //右移一位
                    t3=t3&0x02; //再把t1的最低位压入B0位，对应SEG9位                    
                }else{
                    t3=t3&0x00; //先把t2的最低位压入b1位，对应SEG10位
                    t3=t3<<1;  //右移一位
                    t3=t3&0x02; //再把t1的最低位压入B0位，对应SEG9位
                }
                break;
            case 1:
                if (t2&0x01==1){
                    t3=t3|0x01; //先把t2的最低位压入b1位，对应SEG10位
                    t3=t3<<1;  //右移一位
                    t3=t3|0x01; //再把t1的最低位压入B0位，对应SEG9位                    
                }else{
                    t3=t3&0x00; //先把t2的最低位压入b1位，对应SEG10位
                    t3=t3<<1;  //右移一位
                    t3=t3|0x01; //再把t1的最低位压入B0位，对应SEG9位 
                }
                break;
        }
        WriteAddrData(j*2+1,t3);
        t1=t1>>1; //整体降一位
        t2=t2>>1;        
    }

};
//秒灯闪烁程序，需要配合主程序的设用，blink_flag是控制灯亮与灭的控制标志。
void TM1638::SecondBlink(int blink_flag,int showtime5){
     switch (blink_flag){
        case 0:           
            WriteAddrData(10,tab[showtime5]);  //秒灯在地址0x0A                       
            break;
        case 1:             
            //即B7位要置1，如原来0的显示码是3F，把B7位置1后就变成了B7,这样再显示时秒灯就亮了
            WriteAddrData(10,tab_second[showtime5]);
            break;
    } 
};
