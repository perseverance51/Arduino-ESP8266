//任务：通过按钮控制电机启停和正反转，通过电位计调节电机转速。
int K1=5;  //把K1(正转)按钮连在数字端口5
int K2=6;  //把K2(反转)按钮连在数字端口6
int K3=7;  //把K3(停止)按钮连在数字端口7
int potpin = A3; // 把电位计连在模拟端口A3
int A=2;  //数字端口2、3控制电机启停和转向
int B=3;  
int PWMpin = 9; // 数字端口9输出PWM信号，控制电机转速
//初始化 
void setup() 
{
  pinMode(K1,INPUT);//把数字端口5、6、7设置输入模式
  pinMode(K2,INPUT);
  pinMode(K2,INPUT);
  pinMode(A,OUTPUT);//把数字端口2、3设置输入模式
  pinMode(B,OUTPUT);
}
//主程序 
void loop() 
{
  //如果按下K1(正转)按钮
 if(digitalRead(K1)==LOW)
 {
  //电机正转
  digitalWrite(A,HIGH);
  digitalWrite(B,LOW);
 }
 //如果按下K2(反转)按钮
 if(digitalRead(K2)==LOW)
 {
  //电机反转
  digitalWrite(A,LOW);
  digitalWrite(B,HIGH);
 }
 //如果按下K3(停止)按钮
 if(digitalRead(K3)==LOW)
 {
  //电机停止
  digitalWrite(A,LOW);
  digitalWrite(B,LOW);
 }
 int sensorValue = analogRead(potpin); //读取电位计采样值   
 sensorValue = sensorValue/4; // 采样值 0-1024 转换为 0-255
 analogWrite(PWMpin, sensorValue);//把处理后的转换值以PWM信号形式输出
 delay(20);//延时
}
