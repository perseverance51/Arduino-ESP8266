#include <TM1638lite.h>

TM1638lite tm(4,7, 8);

void setup() {

  tm.reset();

  tm.displayText("Eh");
  tm.setLED(0, 1);
  
  delay(2000);
tm.setLED(2, 1);
  tm.displayASCII(6, 'u');
  tm.displayASCII(7, 'p');
  tm.setLED(7, 1);

  delay(2000);

  tm.displayHex(0, 8);
  tm.displayHex(1, 9);
  tm.displayHex(2, 10);
  tm.displayHex(3, 11);
  tm.displayHex(4, 12);
  tm.displayHex(5, 13);
  tm.displayHex(6, 14);
  tm.displayHex(7, 15);

  delay(2000);

}

void loop() {
for(uint8_t i=0;i<8;i++){
 tm.setLED(i, 1);
 delay(1000); 
}

}
