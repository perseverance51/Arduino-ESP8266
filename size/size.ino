void setup() {
  Serial.begin(115200);
   Serial.println();

}

void loop() {
  unsigned char cha = sizeof(char);
  Serial.print("char sizeof =");
 Serial.println(cha);
   unsigned int INT = sizeof(int);
  Serial.print("unsigned int sizeof =");
Serial.println(INT);
  unsigned char a = 200;
  unsigned char b = 100;
  unsigned char c = 0;
  c = a + b;
   Serial.print("a+b=");
   Serial.println(a+b);
   Serial.print("c=");
   Serial.println(c);  
   delay(6000);
}
