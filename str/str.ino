
const char *str = "perseverance51";
void upper(const char*ptrC);
void setup() {
 Serial.begin(115200);
 Serial.println();
 Serial.println(str);
 Serial.println(&*str);
 Serial.println("start:");
}
void loop() {
 
upper(str);
delay(5000);

}
void upper(const char*ptrC) {
  while (*ptrC) {//遍历字符串成员
   Serial.print(*ptrC++);
//*ptrC += 1;//*ptrC =*ptrC + 1

  }
   Serial.println();
}
