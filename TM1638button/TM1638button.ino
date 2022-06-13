/*
//TM1638
VCC  → 5V
GND → GND
STROBE_TM-----4
CLOCK_TM------6
DIO_TM -------7
*/

#define        DATA_COMMAND        0X40
#define        DISP_COMMAND        0x80
#define        ADDR_COMMAND        0XC0
#define K1S1   1
#define K1S2   2
#define K1S3   3
#define K1S4   4
#define K1S5   5
#define K1S6   6
#define K1S7   7
#define K1S8   8

#define K2S1   9
#define K2S2   10
#define K2S3   11
#define K2S4   12
#define K2S5   13
#define K2S6   14
#define K2S7   15
#define K2S8   16

#define K3S1   17
#define K3S2   18
#define K3S3   19
#define K3S4   20
#define K3S5   21
#define K3S6   22
#define K3S7   23
#define K3S8   24

//TM1638模块引脚定义
int DIO = 7;
int CLK = 6;
int STB = 4; //这里定义了那三个脚
//共阴数码管显示代码
unsigned char tab[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,
                     0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,
                     0x40,0x00
                     };
unsigned char num[8];                //各个数码管显示的值                     
void setup ()
{
  Serial.begin(9600);
  pinMode(STB,OUTPUT);
  pinMode(CLK,OUTPUT);
  pinMode(DIO,OUTPUT); //让三个脚都是输出状态
}
void loop()
    {
        unsigned char i,num;
        init_TM1638();                                   //初始化TM1638
        for(i=0;i<8;i++)
        Write_DATA(i<<1,tab[16]);                       //初始化寄存器       
        while(1)
        {
                i=Read_key();                          //读按键值
                Write_DATA(3<<1,tab[i/10]); 
                Write_DATA(4<<1,tab[i%10]);  
                Serial.println(i);
                delay(1000);
        }
}

void TM1638_Write(unsigned char DATA)  //写数据函数
{
        unsigned char i;
        pinMode(DIO,OUTPUT);
        for(i=0;i<8;i++)
        {
            digitalWrite(CLK,LOW);
            if(DATA&0X01)
               digitalWrite(DIO,HIGH);
            else
               digitalWrite(DIO,LOW);
            DATA>>=1;
            digitalWrite(CLK,HIGH);
        }
}
unsigned char TM1638_Read(void)                                        //读数据函数
{
        unsigned char i;
        unsigned char temp=0;;       
        pinMode(DIO,INPUT);//设置为输入
        for(i=0;i<8;i++)
        {
              temp>>=1;
            digitalWrite(CLK,LOW);
              if(digitalRead(DIO)==HIGH)
                temp|=0x80;
            digitalWrite(CLK,HIGH);
               
        }
        return temp;
}
void Write_COM(unsigned char cmd)                //发送命令字
{
        digitalWrite(STB,LOW);
        TM1638_Write(cmd);
        digitalWrite(STB,HIGH);
}
unsigned char Read_key(void)
{
        unsigned char i,key_val[4],key=0;
        digitalWrite(STB,LOW);
        TM1638_Write(0x42); //读键扫数据 命令
         delayMicroseconds(2);//至少延时1微秒以上
        for(i=0;i<4;i++)               
          {
            key_val[i] =TM1638_Read();
           // Serial.print(key_val[i]);Serial.print('-');
        }
        digitalWrite(STB,HIGH);// 拉高片选线,读键值结束. 
         pinMode(DIO, OUTPUT);//读取完数据出来，设置DIO为输出模式  
   if(key_val[0]!=0) //按键在KS1或KS2
  {
    switch(key_val[0])
    {
      case 1:
        key=K3S1;
      break;      
      case 2:
        key=K2S1;
      break;
      case 4:
        key=K1S1;
      break;
      case 16:
        key=K3S5;
      break;      
      case 32:
        key=K2S5;
      break;     
      case 64:
        key=K1S5;
      break;
    }
  }
  else if(key_val[1]!=0)        //按键在KS3或KS4
  {
    switch(key_val[1])
    {
      case 1:
        key=K3S2;
      break;      
      case 2:
        key=K2S2;
      break;      
      case 4:
        key=K1S2;
      break;
      case 16:
        key=K3S6;
      break;     
      case 32:
        key=K2S6;
      break;    
      case 64:
        key=K1S6;
      break;
      case 255:
      init_TM1638();
       break;
    }
  }
  else if(key_val[2]!=0)        //按键在KS5或KS6
  {
    switch(key_val[2])
    {
      case 1:
        key=K3S3;
      break;     
      case 2:
        key=K2S3;
      break;     
      case 4:
        key=K1S3;
      break;
      case 16:
        key=K3S7;
      break;     
      case 32:
        key=K2S7;
      break;
      case 64:
        key=K1S7;
      break;
      case 255:
      init_TM1638();
       break;
    }
  }
  else if(key_val[3]!=0)        //按键在KS7或KS8
  {
    switch(key_val[3])
    {
      case 1:
        key=K3S4;
      break;      
      case 2:
        key=K2S4;
      break;      
      case 4:
        key=K1S4;
      break;
      case 16:
        key=K3S8;
      break;     
      case 32:
        key=K2S8;
      break;     
      case 64:
        key=K1S8;
      break;
        case 255:
      init_TM1638();
       break;
    }
  }
  
  return key;

}

void Write_DATA(unsigned char addr,unsigned char DATA)//指定地址写入数据
{
        Write_COM(0x44);//向固定地址写数据命令
        digitalWrite(STB,LOW);
        TM1638_Write(0xc0|addr);
        TM1638_Write(DATA);
        digitalWrite(STB,HIGH);
}
/*
void Write_oneLED(unsigned char num,unsigned char flag)  //单独控制一个LED函数，num为需要控制的led序号，flag为0时熄灭，不为0时点亮
{
  if(flag)
    Write_DATA(2*num+1,1);
  else
    Write_DATA(2*num+1,0);
}   */
void Write_allLED(unsigned char LED_flag)  //控制全部LED函数，LED_flag表示各个LED状态
{
        unsigned char i;
        for(i=0;i<8;i++)
        {
            if(LED_flag&(1<<i))
              Write_DATA(2*i+1,1);
            else
              Write_DATA(2*i+1,0);
        }
}

//TM1638初始化函数
void init_TM1638(void)
{
        unsigned char i;
        Write_COM(0x8b);       //亮度 (0x88-0x8f)8级亮度可调
        Write_COM(0x40);       //采用地址自动加1
        digitalWrite(STB,LOW);                           //
        TM1638_Write(0xc0);    //设置起始地址
        for(i=0;i<16;i++)           //传送16个字节的数据
          TM1638_Write(0x00);
        digitalWrite(STB,HIGH);
}
