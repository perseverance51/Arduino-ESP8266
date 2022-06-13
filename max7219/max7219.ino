/*
大方格频谱灯
   项目之四：24段三级联FFT浮点多色频谱灯
*/

#include <arduinoFFT.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#define SAMPLES 64
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES  3
#define CLK_PIN   13
#define DATA_PIN  11
#define CS_PIN    10
#define  xres 24
#define  yres 8


int MY_ARRAY[] = {0, 128, 192, 224, 240, 248, 252, 254, 255};
int MY_MODE_1[] = {0, 128, 192, 224, 240, 248, 252, 254, 255};
int MY_MODE_2[] = {0, 128, 64, 32, 16, 8, 4, 2, 1};
int MY_MODE_3[] = {0, 128, 192, 160, 144, 136, 132, 130, 129};
int MY_MODE_4[] = {0, 128, 192, 160, 208, 232, 244, 250, 253};
int MY_MODE_5[] = {0, 1, 3, 7, 15, 31, 63, 127, 255};


double vReal[SAMPLES];
double vImag[SAMPLES];
char data_avgs[xres];

int yvalue;
int displaycolumn , displayvalue;
int peaks[xres];
const int buttonPin = 2;
int state = HIGH;
int previousState = LOW;
int displaymode = 1;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;


MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
arduinoFFT FFT = arduinoFFT();



void setup() {

  ADCSRA = 0b11100101;
  ADMUX = 0b00000000;
  pinMode(buttonPin, INPUT);
  mx.begin();
  delay(50);
}

void loop() {
  // ++ Sampling
  for (int i = 0; i < SAMPLES; i++)
  {
    while (!(ADCSRA & 0x10));
    ADCSRA = 0b11110101 ;
    int value = ADC - 512 ;
    vReal[i] = value / 8;
    vImag[i] = 0;
  }

  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);

  int step = (SAMPLES / 2) / xres;
  int c = 0;
  for (int i = 0; i < (SAMPLES / 2); i += step)
  {
    data_avgs[c] = 0;
    for (int k = 0 ; k < step ; k++) {
      data_avgs[c] = data_avgs[c] + vReal[i + k];
    }
    data_avgs[c] = data_avgs[c] / step;
    c++;
  }

  for (int i = 0; i < xres; i++)
  {
    data_avgs[i] = constrain(data_avgs[i], 0, 80);
    data_avgs[i] = map(data_avgs[i], 0, 80, 0, yres);
    yvalue = data_avgs[i];

    peaks[i] = peaks[i] - 1;
    if (yvalue > peaks[i])
      peaks[i] = yvalue ;
    yvalue = peaks[i];
    displayvalue = MY_ARRAY[yvalue];
    displaycolumn = 15 - i;
    mx.setColumn(displaycolumn, displayvalue);
  }

  displayModeChange ();
}

void displayModeChange() {
  int reading = digitalRead(buttonPin);
  if (reading == HIGH && previousState == LOW && millis() - lastDebounceTime > debounceDelay)

  {

    switch (displaymode) {
      case 1:
        displaymode = 2;
        for (int i = 0 ; i <= 8 ; i++ ) {
          MY_ARRAY[i] = MY_MODE_2[i];
        }
        break;
      case 2:
        displaymode = 3;
        for (int i = 0 ; i <= 8 ; i++ ) {
          MY_ARRAY[i] = MY_MODE_3[i];
        }
        break;
      case 3:
        displaymode = 4;
        for (int i = 0 ; i <= 8 ; i++ ) {
          MY_ARRAY[i] = MY_MODE_4[i];
        }
        break;
      case 4:
        displaymode = 5;
        for (int i = 0 ; i <= 8 ; i++ ) {
          MY_ARRAY[i] = MY_MODE_5[i];
        }
        break;
      case 5:
        displaymode = 1;
        for (int i = 0 ; i <= 8 ; i++ ) {
          MY_ARRAY[i] = MY_MODE_1[i];
        }
        break;
    }

    lastDebounceTime = millis();
  }
  previousState = reading;
}
