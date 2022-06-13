#include <Adafruit_MPU6050.h>
//#include <Adafruit_SSD1306.h>//0.96寸，启用这个，并注释掉下面的1.3寸的头文件
#include <Adafruit_SH1106.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

//Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);//0.96寸

Adafruit_SH1106 display(-1);//1.3寸，启用这个，注释掉上面这个0.96的头文件

void setup() {
  Serial.begin(115200);
  // while (!Serial);
  Serial.println("MPU6050 OLED demo");

  if (!mpu.begin()) {
    Serial.println("Sensor init failed");
    while (1)
      yield();
  }
  Serial.println("Found a MPU-6050 sensor");
 // if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { //0.96寸，启用if条件语句
//    Serial.println(F("SSD1306 allocation failed"));
//    for (;;)
//      ; // Don't proceed, loop forever
//  }
display.begin(SH1106_SWITCHCAPVCC, 0x3C); //1.3寸，启用这个
  display.display();
  delay(500); // Pause for 2 seconds
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setRotation(0);
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  display.clearDisplay();
  display.setCursor(0, 0);

  Serial.print("加速度 ");
  Serial.print("X: ");
  Serial.print(a.acceleration.x, 1);
  Serial.print(" m/s^2, ");
  Serial.print("Y: ");
  Serial.print(a.acceleration.y, 1);
  Serial.print(" m/s^2, ");
  Serial.print("Z: ");
  Serial.print(a.acceleration.z, 1);
  Serial.println(" m/s^2");

  display.println("Accelerometer - m/s^2");
  display.print(a.acceleration.x, 1);
  display.print(", ");
  display.print(a.acceleration.y, 1);
  display.print(", ");
  display.print(a.acceleration.z, 1);
  display.println("");

  Serial.print("陀螺仪 ");
  Serial.print("X: ");
  Serial.print(g.gyro.x, 1);
  Serial.print(" rps, ");
  Serial.print("Y: ");
  Serial.print(g.gyro.y, 1);
  Serial.print(" rps, ");
  Serial.print("Z: ");
  Serial.print(g.gyro.z, 1);
  Serial.println(" rps");

  display.println("Gyroscope - rps");
  display.print(g.gyro.x, 1);
  display.print(", ");
  display.print(g.gyro.y, 1);
  display.print(", ");
  display.print(g.gyro.z, 1);
  display.println("");

  display.display();
  delay(100);
}
