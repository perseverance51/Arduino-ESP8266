#define led (13)
long num;

void setup() {
  Serial.begin(115200);
  Serial.println();
  pinMode(led,OUTPUT);
  digitalWrite(led,HIGH);
  num = 32767L + 1L;

}

void loop() {
  Serial.println(num);
  delay(3000);

}
