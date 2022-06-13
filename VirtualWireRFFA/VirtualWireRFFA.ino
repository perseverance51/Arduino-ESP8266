/*
 发射模块  VCC GND  DATA- arduino nano D12脚
 LED为板载led

*/

#include <VirtualWire.h>

void setup()
{
  Serial.begin(9600);  // Debugging only
  Serial.println("STAR"); // 
  vw_set_tx_pin(12);     //  D12作为 TX引脚，也就是接RF发送模块的DATA引脚
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(4000);     // Bits per sec
}

void loop()
{
  const char *msg = "perseverance9527";  // Message to be sent
  digitalWrite(13, true);   // Flash a light to show transmitting
  vw_send((uint8_t *)msg, strlen(msg)); //Sending the message
  vw_wait_tx(); // Wait until the whole message is gone
  digitalWrite(13, false);  // Turn the LED off.
  delay(2000);         // A short gap.
}
