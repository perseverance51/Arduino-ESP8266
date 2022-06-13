long num=0;
bool flag = false;
//int NUM[20];
/*int NUM[20];
 * 项目使用了 262312 字节，占用了 (25%) 程序存储空间。最大为 1044464 字节。
全局变量使用了26884字节，(32%)的动态内存，余留55036字节局部变量。最大为81920字节。
 */
 
 int *Buffer = new int [20];
 
 /*int *Buffer = new int [20]
  * 项目使用了 262396 字节，占用了 (25%) 程序存储空间。最大为 1044464 字节。
全局变量使用了26896字节，(32%)的动态内存，余留55024字节局部变量。最大为81920字节。
  * 
  */

void setup() {
  Serial.begin(115200);
}

void loop() {
  while (Serial.available() > 0) {   // 串口收到字符数大于零。
    num = Serial.parseInt();      // 在串口数据流中查找一个有效整数。  
   Serial.println(num); 
  while(Serial.read() >= 0){}//清空串口接收的数据
  }
}
