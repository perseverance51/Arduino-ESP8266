
void setup()
{
    Serial.begin(115200);
 //  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);//仅开启串口输出模式
 // Serial.begin(115200, SERIAL_8N1, SERIAL_RX_ONLY);//仅开启串口输入模式,无法串口打印输出信息
 // Serial.begin(115200, SERIAL_8N1, SERIAL_FULL);//默认模式
 //  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY, 1);   //     void begin(unsigned long baud, SerialConfig config, SerialMode mode, uint8_t tx_pin);
Serial.setDebugOutput(true);  //将串口设为调试输出模式
}
void loop()
{
   printf("Hello");
    for(int i=0; i<5000; i++)
    {
        delayMicroseconds(1000);
        yield();
    }
}
