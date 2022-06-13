
 struct People {
  char *name;
  int age;
  float Height;
} ;//注：建议不要将`*stu`结构体变量放在这里。(这里不是`People`的别名!!!)

void setup() {
  Serial.begin(115200);
  Serial.flush();
//char *p= (char*)malloc(10*sizeof(char));/*结构体指针的成员指针同样需要初始化*/ 
   struct People *stu;  
  stu->age = 12;
  stu->Height = 46.2;
  stu->name = "chen";
  
  Serial.println(stu->age);Serial.println(stu->Height);
 // stu->name = (char*)malloc(10*sizeof(char));
  Serial.println(stu->name);
}
void loop() {
// Serial.println(stu->age);Serial.println(stu->Height);Serial.println(stu->name);
  delay(5000);
}
