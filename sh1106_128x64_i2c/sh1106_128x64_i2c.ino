/*********************************************************************
This is an example for our Monochrome OLEDs based on SH1106 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98
*********************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include "arduinoFFT.h"
#define OLED_RESET -1
Adafruit_SH1106 display(OLED_RESET);

#if (SH1106_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SH1106.h!");
#endif

#define SAMPLES 64 // power of 2
#define SAMPLING_FREQ 16000 //8000 12 kHz Fmax = sampleF /2
#define AMPLITUDE 100 // 灵敏度
#define FREQUENCY_BANDS 14  //14

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define BARWIDTH 11 //每条宽度
#define BARS 11    //分成8等分
#define ANALOG_PIN A0

double vImag[SAMPLES];
double vReal[SAMPLES];
unsigned long sampling_period_us;
arduinoFFT fft = arduinoFFT(vReal, vImag,SAMPLES, SAMPLING_FREQ);
//arduinoFFT fft = arduinoFFT();
//调整参考以去除背景噪声
float reference = log10(60.0);//60.0
double coutoffFrequencies[FREQUENCY_BANDS];

int oldHeight[20];
int oldMax[20];
double maxInFreq;

void setup()   {                
  Serial.begin(115200);
randomSeed(analogRead(ANALOG_PIN));
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SH1106_SWITCHCAPVCC, 0x3c);  // initialize with the I2C addr 0x3D (for the 128x64)
 display.invertDisplay(false);
  // Clear the buffer.
  display.clearDisplay();
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
      display.drawPixel((i + 1)*BARWIDTH + 2 , j, WHITE );
    }
  }
  display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
    display.display();
}


void loop() {
  // 采样
  for (int i = 0; i < SAMPLES; i++) {
    unsigned long newTime = micros();
    //int value = analogRead(ANALOG_PIN);
    int value = random(1024);
    vReal[i] = value;
    vImag[i] = 0;
    while (micros() < (newTime + sampling_period_us)) {
     // yield();
     delay(50);
    }
  }
  // 计算 FFT
  fft.DCRemoval();
  fft.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  fft.Compute(FFT_FORWARD);
  fft.ComplexToMagnitude();
  double median[20];
  double max[20];
  int index = 0;
  double hzPerSample = (1.0 * SAMPLING_FREQ) / SAMPLES; //
  double hz = 0;
  double maxinband = 0;
  double sum = 0;
  int count = 0;
  for (int i = 2; i < (SAMPLES / 2) ; i++) {
    count++;
    sum += vReal[i];
    if (vReal[i] >  max[index] ) {
      max[index] = vReal[i];
    }
    if (hz > coutoffFrequencies[index]) {
      median[index] = sum / count;
      sum = 0.0;
      count = 0;
      index++;
      max[index] = 0;
      median[index]  = 0;
    }
    hz += hzPerSample;
  }
  // 计算每个频段的中值和最大值
  if ( sum > 0.0) {
    median[index] =  sum / count;
    if (median[index] > maxinband) {
      maxinband = median[index];
    }
  }
  int bar = 0;
  for (int i = FREQUENCY_BANDS - 1; i >= 3; i--) {
    int newHeight = 0;
    int newMax = 0;
    // 计算实际分贝
    if (median[i] > 0 && max[i] > 0 ) {
      newHeight = 20.0 * (log10(median[i] ) - reference);
      newMax = 20.0 * (log10(max[i] ) - reference);
    }
    // 调整最小和最大级别
    if (newHeight < 0 ||  newMax < 0) {
      newHeight = 1;
      newMax = 1;
    }
    if (newHeight >= SCREEN_HEIGHT - 2) {
      newHeight = SCREEN_HEIGHT - 3;
    }
    if (newMax >= SCREEN_HEIGHT - 2) {
      newMax = SCREEN_HEIGHT - 3;
    }
    int barX = bar * BARWIDTH + 5;
    // 删除旧水平中位数
    if (oldHeight[i] > newHeight) {
      display.fillRect(barX, newHeight + 1, 7, oldHeight[i], BLACK);
       // display.display();
    }
    // 删除旧的最大级别
    if ( oldMax[i] > newHeight) {
      for (int j = oldMax[i]; j > newHeight; j -= 2) {
        display.drawFastHLine(barX , j, 7, BLACK);         
      }
       display.display();

    }
    // 绘制新的最大级别
    for (int j = newMax; j > newHeight; j -= 2) {
      display.drawFastHLine(barX , j, 7,  WHITE);
    }
     display.display();
    // 绘制新的级别中位数
    display.fillRect(barX , 1, 7, newHeight, WHITE);
    oldMax[i] = newMax;
    oldHeight[i] = newHeight;
    bar++;
  }
  display.drawFastHLine(0 , SCREEN_HEIGHT - 1, SCREEN_WIDTH, WHITE);
  display.display();
//  display.clearDisplay();
memset(oldMax,0,sizeof(oldHeight));
memset(oldHeight,0,sizeof(oldHeight));
}
