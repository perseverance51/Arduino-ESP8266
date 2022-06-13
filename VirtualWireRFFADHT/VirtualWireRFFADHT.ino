/*
  发射模块  VCC GND  DATA- arduino nano D12脚
  LED为板载led

*/

#include <VirtualWire.h>
#include "DHT.h";

#define DHTPIN 7       // DHT-11 Output Pin connection
#define DHTTYPE DHT11
float hum;    // Stores humidity value in percent
float temp;
String str_humid;
String str_temp;
String str_out;
DHT dht(DHTPIN, DHTTYPE);
void setup()
{
  dht.begin();
  Serial.begin(9600);  // Debugging only
  Serial.println("STAR"); //
  vw_set_tx_pin(12);     //  D12作为 TX引脚，也就是接RF发送模块的DATA引脚
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(4000);     // Bits per sec
}

void loop()
{
  //  const char *msg = "perseverance9527";  // Message to be sent
  //  digitalWrite(13, true);   // Flash a light to show transmitting
  delay(500);
  hum = dht.readHumidity();  // Get Humidity value
  temp = dht.readTemperature();
  delay(2000);  // Delay so DHT-11 sensor can stabalize
  // 湿度数据转换
  str_humid = String(hum);
  // 温度数据转换
  str_temp = String(temp);
  // 将湿度和温度拼接成一体
  str_out = "RH："+ str_humid +"%" "," +"Temp:"+ str_temp + "℃";
  // 转换为RF射频模块发送数据类型
  static char *msg = str_out.c_str();
  vw_send((uint8_t *)msg, strlen(msg)); //Sending the message
  vw_wait_tx(); // Wait until the whole message is gone
  digitalWrite(13, false);  // Turn the LED off.

}
