void setup()
{
Serial.begin(9600);
}

void loop()
{
String comdata="";//使用字符串来接收，并初始化。
while (Serial.available() > 0)
{//判断有数据进来
comdata += char(Serial.read());//将接收到的字符串连起来，数据类型强制转换
delay(2);
}
if (comdata.length() > 0)
{//通过判断变量的数据长度，拉打印输出
Serial.print("串口接收到的数据:");
Serial.println(comdata);
comdata = "";//清空变量数据
}
}
