/*
 * 一个英文字符等于1个字节，一个中文（含繁体）字符等于3个字节；中文标点占3个字节，英文标点占1个字节。
 * 1Byte=8bit;
 * char = 8bit
 */
void libian(String str0);

void setup() {
Serial.begin(115200);
String strn= "白日依山尽，黄河入海流。欲穷千里目，更上一层楼。";

Serial.println(strn.length());
Serial.println(strn);
libian(strn);
char ch2[64];
strcpy(ch2,strn.c_str());
Serial.println(ch2);
char ch3[64];
strn.getBytes(ch3,strn.length()+1);//strn.length()等价于strlen()函数，长度不包含‘\0’
Serial.println(ch3);
}

void loop() {

}

void libian(String str0)
{
  int temp = 0;
  String str = str0;
  if (str.indexOf('，') != -1)
  {
    temp++;
    str.replace("，", ",");
  }
  if (str.indexOf('；') != -1)
  {
    temp++;
    str.replace("；", ";");
  }
  if (str.indexOf('。') != -1)
  {
    temp++;
    str.replace("。", ".");
  }
  if (str.indexOf('“') != -1)
  {
    temp++;
    str.replace("“", "\"");
  }
  if (str.indexOf('”') != -1)
  {
    temp++;
    str.replace("”", "\"");
  }
  Serial.println();
  Serial.println(str);
  Serial.println(str.length());
//  int jishu = (str0.length() - str.length()) / 3;//计算标点符号字符个数
char ch[66];
str.toCharArray(ch,str.length()+1);//字符串转字符数组
  Serial.println(ch);
}
