/**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers

 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98

 This example is for a 128x64 pixel display using I2C to communicate
 3 pins are required to interface (two I2C and one reset).

 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 **************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "arduinoFFT.h"
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3c ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define SAMPLES 64 // power of 2
#define SAMPLING_FREQ 5000 //8000 12 kHz Fmax = sampleF /2
#define AMPLITUDE 100 // 灵敏度
#define FREQUENCY_BANDS 7  //14

#define BARWIDTH 11
#define BARS 11
#define ANALOG_PIN A0

double vImag[SAMPLES];
double vReal[SAMPLES];
unsigned long sampling_period_us;
//arduinoFFT fft = arduinoFFT(vReal, vImag, SAMPLES, SAMPLING_FREQ);
arduinoFFT fft = arduinoFFT();
//调整参考以去除背景噪声
float reference = log10(60.0);//60.0
double coutoffFrequencies[FREQUENCY_BANDS];

int oldHeight[20];
int oldMax[20];
double maxInFreq;

void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
  display.drawPixel(10, 10, SSD1306_WHITE);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(2000);
 display.clearDisplay();
  display.display();
  display.setRotation(0);
  display.invertDisplay(false);
  sampling_period_us = (1.0 / SAMPLING_FREQ ) * pow(10.0, 6);
  // 计算截止频率，以对数标度为基数 POt
  double basePot = pow(SAMPLING_FREQ / 2.0, 1.0 / FREQUENCY_BANDS);
  coutoffFrequencies[0] = basePot;
  for (int i = 1 ; i < FREQUENCY_BANDS; i++ ) {
    coutoffFrequencies[i] = basePot * coutoffFrequencies[i - 1];
  }
  // 绘制虚线以分离频段
  for (int i = 0; i < BARS - 1 ; i++) {
    for (int j = 0; j < SCREEN_HEIGHT ; j += 4) {
      display.writePixel((i + 1)*BARWIDTH + 2 , j, SSD1306_WHITE );
    }
  }
  display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);

}

void loop() {
}
