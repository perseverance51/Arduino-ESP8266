#define led (2)
int num;

void setup() {
  Serial.begin(115200);
  Serial.println();
  pinMode(led,OUTPUT);

}

void loop() {
if(Serial.available() > 0){
num = Serial.parseInt();
Serial.println(num);
  }
if(num > 0){
  num --;
  digitalWrite(led,HIGH);
  delay(1000);
  digitalWrite(led,LOW);
  delay(1000);
}
}
