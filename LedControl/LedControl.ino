#include <LedControl.h>
const int DIN = 11;//dataPin
const int CLK = 9;//
const int CS = 10;//LOAD
int maxLen;//字的总个数
int row;//纵向移动数组的行数
int col;//横向移动数组的列数
//自定义函数
void showWordByArr(LedControl led, byte[][2]); //通过数组显示
void showWordByPtr(LedControl led, byte(*word)[2]); //通过数组指针显示
void showWords(LedControl led, byte(*words)[16][2]); //显示一组文字
void showTopMoveWords(LedControl led, byte(*words)[16][2]); //向上移动显示文字
void showLeftMoveWords(LedControl led, byte(*words)[16][2]); //向左移动显示文字

//初始化对象，每个对象最多可以操作8个设备(超过8个设备就再创建一个对象)。
LedControl led = LedControl(DIN, CLK, CS , 4);//我这里是4块8*8的led点阵，所以设备数量填4(最大为8)。
byte a[][2] =  //实心五角星
{
  0x01, 0x00, //第一，二个设备
  0x01, 0x00,
  0x03, 0x80,
  0x03, 0x80,
  0x03, 0x80,
  0x07, 0xC0,
  0xFF, 0xFE,
  0x7F, 0xFC,
  0x3F, 0xF8, //第三，四个设备
  0x0F, 0xE0,
  0x0F, 0xE0,
  0x1F, 0xF0,
  0x1E, 0xF0,
  0x18, 0x30,
  0x20, 0x08,
  0x00, 0x00
};
byte b[][2] =  //空心五角星
{
  0x01, 0x00, 0x01, 0x00, 0x02, 0x80, 0x02, 0x80, 0x02, 0x80, 0x04, 0x40, 0xFC, 0x7E, 0x40, 0x04,
  0x30, 0x18, 0x08, 0x20, 0x08, 0x20, 0x11, 0x90, 0x16, 0x50, 0x18, 0x30, 0x20, 0x08, 0x00, 0x00
};
byte c[][2] =  //空心取反
{
  0xFE, 0xFF, 0xFE, 0xFF, 0xFC, 0x7F, 0xFC, 0x7F, 0xFC, 0x7F, 0xF8, 0x3F, 0x00, 0x01, 0x80, 0x03,
  0xC0, 0x07, 0xF0, 0x1F, 0xF0, 0x1F, 0xE0, 0x0F, 0xE1, 0x0F, 0xE7, 0xCF, 0xDF, 0xF7, 0xFF, 0xFF
};

byte topWords[][16][2] = //庆祝祖国70周年
{
  {0x00},
  {
      0x01, 0x00, 0x00, 0x80, 0x3F, 0xFE, 0x20, 0x00, 0x20, 0x80, 0x20, 0x80, 0x20, 0x80, 0x2F, 0xFC,
      0x20, 0x80, 0x21, 0x40, 0x21, 0x40, 0x22, 0x20, 0x42, 0x20, 0x44, 0x10, 0x88, 0x08, 0x10, 0x06
  },
  {
      0x20, 0x00, 0x13, 0xFC, 0x12, 0x04, 0xFA, 0x04, 0x0A, 0x04, 0x12, 0x04, 0x13, 0xFC, 0x38, 0x90,
      0x54, 0x90, 0x94, 0x90, 0x10, 0x90, 0x11, 0x12, 0x11, 0x12, 0x12, 0x12, 0x14, 0x0E, 0x18, 0x00
  },
  {
      0x20, 0x00, 0x11, 0xF8, 0x11, 0x08, 0xF9, 0x08, 0x09, 0x08, 0x11, 0xF8, 0x11, 0x08, 0x39, 0x08,
      0x55, 0x08, 0x95, 0xF8, 0x11, 0x08, 0x11, 0x08, 0x11, 0x08, 0x11, 0x08, 0x17, 0xFE, 0x10, 0x00
  },
  {
      0x00, 0x00, 0x7F, 0xFC, 0x40, 0x04, 0x40, 0x04, 0x5F, 0xF4, 0x41, 0x04, 0x41, 0x04, 0x4F, 0xE4,
      0x41, 0x04, 0x41, 0x44, 0x41, 0x24, 0x5F, 0xF4, 0x40, 0x04, 0x40, 0x04, 0x7F, 0xFC, 0x40, 0x04
  },
  {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x18, 0x44, 0x24, 0x44, 0x42, 0x08, 0x42, 0x08, 0x42,
      0x10, 0x42, 0x10, 0x42, 0x10, 0x42, 0x10, 0x24, 0x10, 0x18, 0x00, 0x00, 0x00, 0x00
  },
  {
      0x00, 0x00, 0x3F, 0xF8, 0x21, 0x08, 0x21, 0x08, 0x2F, 0xE8, 0x21, 0x08, 0x21, 0x08, 0x3F, 0xF8,
      0x20, 0x08, 0x27, 0xC8, 0x24, 0x48, 0x24, 0x48, 0x27, 0xC8, 0x40, 0x08, 0x40, 0x28, 0x80, 0x10
  },
  {
      0x10, 0x00, 0x10, 0x00, 0x1F, 0xFC, 0x20, 0x80, 0x20, 0x80, 0x40, 0x80, 0x1F, 0xF8, 0x10, 0x80,
      0x10, 0x80, 0x10, 0x80, 0xFF, 0xFE, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80
  },
  {
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x30,0x00,0x10,0x00,0x20,0x00,0x00,0x00
  }

};
byte words[][16][2]= //愿祖国繁荣昌盛，越来越好,
{
    {
        0x00,0x00,0x3F,0xFE,0x20,0x80,0x27,0xF8,0x24,0x08,0x27,0xF8,0x24,0x08,0x27,0xF8,
        0x22,0x50,0x24,0x48,0x29,0x44,0x20,0x80,0x20,0x44,0x4A,0x42,0x4A,0x12,0x91,0xF0
    },
    {
        0x20,0x00,0x11,0xF8,0x11,0x08,0xF9,0x08,0x09,0x08,0x11,0xF8,0x11,0x08,0x39,0x08,
        0x55,0x08,0x95,0xF8,0x11,0x08,0x11,0x08,0x11,0x08,0x11,0x08,0x17,0xFE,0x10,0x00
    },
    {
        0x00,0x00,0x7F,0xFC,0x40,0x04,0x40,0x04,0x5F,0xF4,0x41,0x04,0x41,0x04,0x4F,0xE4,
        0x41,0x04,0x41,0x44,0x41,0x24,0x5F,0xF4,0x40,0x04,0x40,0x04,0x7F,0xFC,0x40,0x04
    },
    {
        0x20,0x20,0x3F,0x20,0x40,0x3E,0xBE,0x48,0x2A,0xA8,0xFF,0x28,0x4A,0x10,0x7F,0x28,
        0x04,0x46,0x08,0x80,0x1F,0x00,0x02,0x10,0x3F,0xF8,0x01,0x08,0x15,0x20,0x22,0x10
    },
    {
        0x08,0x20,0x08,0x20,0xFF,0xFE,0x08,0x20,0x00,0x00,0x7F,0xFE,0x40,0x02,0x81,0x04,
        0x01,0x00,0x7F,0xFC,0x03,0x80,0x05,0x40,0x09,0x20,0x31,0x18,0xC1,0x06,0x01,0x00
    },
    {
        0x1F,0xF0,0x10,0x10,0x10,0x10,0x1F,0xF0,0x10,0x10,0x10,0x10,0x1F,0xF0,0x00,0x00,
        0x3F,0xF8,0x20,0x08,0x20,0x08,0x3F,0xF8,0x20,0x08,0x20,0x08,0x3F,0xF8,0x20,0x08
    },
    {
        0x00,0x90,0x00,0x88,0x3F,0xFC,0x20,0x80,0x20,0x88,0x3E,0x48,0x22,0x50,0x22,0x24,
        0x4A,0x54,0x44,0x8C,0x81,0x04,0x3F,0xF8,0x24,0x48,0x24,0x48,0xFF,0xFE,0x00,0x00
    },
    {
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x30,0x00,0x10,0x00,0x20,0x00,0x00,0x00
    },
    {
        0x10,0x28,0x10,0x24,0x10,0x20,0x7D,0xFC,0x11,0x20,0x11,0x24,0xFD,0x24,0x11,0x28,
        0x11,0x28,0x51,0x10,0x5D,0x94,0x51,0x2C,0x70,0x44,0x50,0x00,0x4F,0xFE,0x80,0x00
    },
    {
        0x01,0x00,0x01,0x00,0x01,0x00,0x7F,0xFC,0x01,0x00,0x11,0x10,0x09,0x10,0x09,0x20,
        0xFF,0xFE,0x03,0x80,0x05,0x40,0x09,0x20,0x31,0x18,0xC1,0x06,0x01,0x00,0x01,0x00
    },
    {
        0x10,0x28,0x10,0x24,0x10,0x20,0x7D,0xFC,0x11,0x20,0x11,0x24,0xFD,0x24,0x11,0x28,
        0x11,0x28,0x51,0x10,0x5D,0x94,0x51,0x2C,0x70,0x44,0x50,0x00,0x4F,0xFE,0x80,0x00
    },
    {
        0x10,0x00,0x10,0xFC,0x10,0x04,0x10,0x08,0xFC,0x10,0x24,0x20,0x24,0x20,0x25,0xFE,
        0x24,0x20,0x48,0x20,0x28,0x20,0x10,0x20,0x28,0x20,0x44,0x20,0x84,0xA0,0x00,0x40
    },
    {
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x30,0x00,0x10,0x00,0x20,0x00,0x00,0x00
    }
};
byte leftWords[][16][2]=  //我爱你中国！
{
    {
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x1A,0x1C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
    },
    {
        0x04,0x24,0x24,0x24,0x7F,0xC4,0x44,0x04,0x04,0xFF,0x04,0x44,0x34,0x05,0x04,0x00,
        0x00,0x10,0x12,0x21,0xFE,0x40,0x82,0x02,0x04,0xC8,0x30,0x28,0x44,0x82,0x1F,0x00
    },
    {
        0x01,0x26,0x34,0x2C,0x24,0x24,0x37,0x4C,0x44,0x44,0x4C,0x74,0xC4,0x45,0x06,0x00,
        0x00,0x82,0x84,0x89,0x91,0xE1,0xB2,0xAA,0xA4,0xA4,0xAA,0xB2,0x81,0x01,0x01,0x00
    },
    {
        0x00,0x01,0x06,0x1F,0xE0,0x02,0x04,0x18,0xF0,0x10,0x13,0x10,0x10,0x14,0x18,0x00,
        0x80,0x00,0x00,0xFF,0x00,0x08,0x30,0xC0,0x02,0x01,0xFE,0x00,0x80,0x60,0x18,0x00
    },
    {
        0x00,0x00,0x0F,0x08,0x08,0x08,0x08,0xFF,0x08,0x08,0x08,0x08,0x0F,0x00,0x00,0x00,
        0x00,0x00,0xF0,0x20,0x20,0x20,0x20,0xFF,0x20,0x20,0x20,0x20,0xF0,0x00,0x00,0x00
    },
    {
        0x00,0x7F,0x40,0x48,0x49,0x49,0x49,0x4F,0x49,0x49,0x49,0x48,0x40,0x7F,0x00,0x00,
        0x00,0xFF,0x02,0x12,0x12,0x12,0x12,0xF2,0x12,0x52,0x32,0x12,0x02,0xFF,0x00,0x00
    },
    {
        0x00,0x00,0x00,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0xCC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
    }
};


void setup() {
  for (int i = 0; i < 4; ++i)
  {
    led.shutdown(i, false);//关闭省电模式，这一步必须做，否则显示不出来(因为在构造函数中，默认开启了省电模式，所以，我们要关闭该模式)。
   led.setIntensity(i,18);       //0-15将亮度设置为最大值
    led.clearDisplay(i);//清空数据
  }
  maxLen = sizeof(words)/sizeof(*words);//计算总的字数
  row = 16 * (sizeof(topWords) / sizeof(*topWords));//计算向上移动的字的总行数
  col = 16 * (sizeof(leftWords) / sizeof(*leftWords));//计算向左移动的字的总列数
}

void loop() {
  // put your main code here, to run repeatedly:
  showWordByPtr(led, b); //显示空心五角星
  delay(1000);
  for(int i =0;i<5;++i)
  {
    showWordByArr(led, a); //实心五角星
    delay(1000);
    showWordByArr(led, c); //空心取反
    delay(1000);
  }
  showWordByArr(led, a); //实心五角星
  delay(1000);
  showTopMoveWords(led, topWords); //字向左滚动
  delay(1000);
  showWords(led, words); //字原地变换
  delay(500);
  showLeftMoveWords(led, leftWords); //字向右滚动
  delay(1000);
}

void showWordByArr(LedControl led, byte word[][2])
{
  for (int i = 0; i < 4; ++i)
  {
    if (i < 2)
    {
      for (int j = 0; j < 8; ++j)
      {
        led.setRow(i, j, word[j][i]);
      }
    }
    else
    {
      for (int j = 0; j < 8; ++j)
      {
        led.setRow(i, j, word[j + 8][i - 2]);
      }
    }
  }
}

void showWordByPtr(LedControl led, byte (*word)[2])
{
  for (int i = 0; i < 4; ++i)
  {
    if (i < 2)
    {
      for (int j = 0; j < 8; ++j)
      {
        led.setRow(i, j, *(*(word + j) + i));
      }
    }
    else
    {
      for (int j = 0; j < 8; ++j)
      {
        led.setRow(i, j, *(*(word + j + 8) + i - 2));
      }
    }
  }
}

void showWords(LedControl led, byte (*words)[16][2])
{
  for (int i = 0; i < maxLen; ++i)
  {
    showWordByPtr(led, *(words + i));
    delay(800);
  }
}

void showTopMoveWords(LedControl led, byte(*words)[16][2])
{
  byte *temp = *(*words); //获取第一个二维数组的第一行的(第一个元素)的指针
  byte* dump = temp;
  for(int n =0;n<=row-16;++n)
  {
    dump = temp;
    for (int i = 0; i < 4; ++i)//共4块8*8led点阵，每块显示完再到下一块
    {
      dump = temp;
      if (i < 2)  //第一、二块的点阵
      {
        for (int j = 0; j < 8; ++j)  //每块点阵共8行，所以循环8次
        {
          led.setRow(i, j, *(dump + i));
          dump+=2;  //指向下一行的第一个位置
        }
      }
      else  //第三、四块点阵显示
      {
        dump+=16;  //因为是显示二维数组的后半部分，这里要加一下。
        for (int j = 0; j < 8; ++j)
        {
          led.setRow(i, j, *(dump+i-2));
          dump+=2;
        }
      }
    }
    temp += 2;//每次循环完，指向下一行。
    delay(35); //每次显示之间间隔35ms,这是我自己测试的，如果想要更快或更慢可以相应调整这个参数。
  }
}

void showLeftMoveWords(LedControl led, byte(*words)[16][2])
{
    byte* first = *(*words);
    byte* firstChangeFlag = first + 15;//开始遍历的指针需要变换的标志(也就是当first和firstChangeFlag相等时，说明first需要再偏移17).
    byte* temp = first; //负责显示第一、二个led点阵的指针
    byte* temp1 = first + 16; //负责显示第三、四个点阵的指针
    for (int n = 0; n < col - 16; ++n)
    {
        temp = first;
        temp1 = first + 16;
        for (int i=0;i<4;++i)
        {
            if (i < 2) //第一、二个点阵
            {
                for (int j = 0; j < 8; ++j)
                {
                    led.setColumn(i, j, *temp);
                    if (temp == firstChangeFlag)
                    {
                        temp += 17;
                    }
                    else
                    {
                        temp++;
                    }
                }
            }
            else  //第三、四个点阵
            {
                for (int k=0; k<8; ++k)
                {
                    led.setColumn(i, k, *temp1);
                    if (temp1 == firstChangeFlag+16)
                    {
                        temp1 += 17;
                    }
                    else
                    {
                        temp1++;
                    }
                }
            }
        }
        if (first == firstChangeFlag)
        {
            first += 17;
            firstChangeFlag = first + 15;
        }
        else
        {
            first++;
        }
        //delay(1000);
    }
}