

/*
 * 接线方式：

GND、VCC（+5V）

VRx，VRy （X、Y轴）为模拟输入信号，连接到模拟IO口A0~A7。
VRx，VRy 的值：从 0 ~ 1023 分别代表 左~右，上~下。中间值为512。

SW （Z轴）是数字输入信号，连接到数字端口，并启用上拉电阻。
SW 的值：1代表未按下，0代表按下。
//在这里VRx(x)连的是模拟端口A0，VRy(y)连的是模拟端口A1，SW(z)连接到了数字端口IO 7
 */

int value = 0; 
void setup() { 
 pinMode(7, INPUT_PULLUP); //注意 Z 轴输入一定要上拉，不然电平不稳。
 Serial.begin(9600); 
} 
void loop() { 
 value = analogRead(A0); 
 Serial.print("X:"); 
 Serial.print(value, DEC); 
 value = analogRead(A1); 
 Serial.print(" | Y:"); 
 Serial.print(value, DEC); 
 value = digitalRead(7); 
 Serial.print(" | Z: "); 
 Serial.println(value, DEC); 
 delay(500); 
} 
