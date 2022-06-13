void setup() {
Serial.begin(115200);

for(int i=0;i<8;i++){
 uint8_t k = 0xff;
   k=bitWrite(k,i,0);
Serial.print("k二进制表示：");
Serial.print(k,BIN);  
Serial.print("  k十六进制表示："); 
Serial.println(k,HEX);
}
delay(500);
}

void loop() {

}
