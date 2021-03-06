/*
  433 MHz RF Module Transmitter Demonstration 2
  RF-Xmit-Demo-2.ino
  Demonstrates 433 MHz RF Transmitter Module with DHT-11 Sensor
  Use with Receiver Demonstration 2
  RF433数据引脚DATE接Arduino nano上的D11引脚（11）
  DHT11的数据引脚接Arduino nano上的D7引脚（7）
*/

// Include RadioHead Amplitude Shift Keying Library
#include <RH_ASK.h>
// Include dependant SPI Library 
#include <SPI.h> 

// Include DHT Libraries from Adafruit
// Dependant upon Adafruit_Sensors Library
#include "DHT.h";

// Define Constants

#define DHTPIN 7       // DHT-11 Output Pin connection
#define DHTTYPE DHT11   // DHT Type is DHT 11 (AM2302)

// Define Variables

float hum;    // Stores humidity value in percent
float temp;   // Stores temperature value in Celcius

// Define output strings

String str_humid;
String str_temp;
String str_out;

// Create Amplitude Shift Keying Object
RH_ASK rf_driver;

// Initialize DHT sensor for normal 16mhz Arduino
DHT dht(DHTPIN, DHTTYPE);

void setup() {

  // Initialize ASK Object
  rf_driver.init();
  // Start DHT Sensor
  dht.begin();

}

void loop()
{

  delay(2000);  // Delay so DHT-22 sensor can stabalize
   
    hum = dht.readHumidity();  // Get Humidity value
    temp= dht.readTemperature();  // Get Temperature value
    
    // Convert Humidity to string
    str_humid = String(hum);
    
    // Convert Temperature to string
    str_temp = String(temp);

    // Combine Humidity and Temperature
    str_out = str_humid + "," + str_temp;
    
    // Compose output character
    static char *msg = str_out.c_str();
    
    rf_driver.send((uint8_t *)msg, strlen(msg));
    rf_driver.waitPacketSent();
  
}
