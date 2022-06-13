const byte GPIOPins[] = { 2, 6, 7, 8, 9, 12 };
const signed char GPIOPins2[] = { 2, 6, 7, 8, 9, 12 };

void setup() {
  Serial.begin(115200);
  for (uint8_t i = 0; i < 6; i++) {
    pinMode(GPIOPins[6], OUTPUT);
    digitalWrite(GPIOPins[6], LOW);
  }

}

void loop() {
  uint8_t charsize = sizeof(GPIOPins2);
  uint8_t bytesize = sizeof(GPIOPins);

  Serial.print("charsize =");
  Serial.println(charsize);

  Serial.print("bytesize = " );
  Serial.println(bytesize);

  delay(5000);
}
