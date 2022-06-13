/*

接收模块 VCC GND  DATA- arduino nano D11脚

*/
#include <VirtualWire.h>

int count;
void setup(){
  Serial.begin(9600); // Debugging only
  Serial.println("setup"); //Prints "Setup" to the serial monitor
  vw_set_rx_pin(11);    // D12作为 RX引脚，也就是接RF接收模块的DATA引脚
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(4000);   // Bits per sec
  vw_rx_start();    // Start the receiver PLL running
}



void loop(){
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  if (vw_get_message(buf, &buflen)) // Non-blocking
  {
 int i;
    digitalWrite(13, true); // Flash a light to show received good message
 // Message with a good checksum received, dump it.
 Serial.print("Got: ");
 for (i = 0; i < buflen; i++)
 {
      char c = buf[i];
      Serial.print(c);
  // Serial.print(" ");
 }
   count++;
    // Serial.print(count);
 Serial.println("");
    digitalWrite(13, false);
  }

}
