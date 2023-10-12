#include <Arduino.h>
#include <M5Stack.h>

uint16_t HVtoRGB565(float H, float V)
{
  int r, g, b;

  int i = (int)(H * 6.0); // 色相を6つのセクタに分割
  float f = H * 6.0 - i;
  float p = V * (1.0 - f);
  float q = V * (1.0 - (1.0 - f));
  float t = V * f;

  switch (i % 6)
  {
  case 0:
    r = V * 31;
    g = t * 63;
    b = p * 31;
    break;
  case 1:
    r = q * 31;
    g = V * 63;
    b = p * 31;
    break;
  case 2:
    r = p * 31;
    g = V * 63;
    b = t * 31;
    break;
  case 3:
    r = p * 31;
    g = q * 63;
    b = V * 31;
    break;
  case 4:
    r = t * 31;
    g = p * 63;
    b = V * 31;
    break;
  case 5:
    r = V * 31;
    g = p * 63;
    b = q * 31;
    break;
  }

  // 16ビットのRGB565に変換
  return (r << 11) | (g << 5) | b;
}

void setup()
{
  M5.begin();

  M5.Lcd.begin();
  M5.Lcd.setRotation(1);
  M5.Lcd.setTextWrap(false);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setTextColor(TFT_BLACK, TFT_BLACK);

  int startX = 50;               // 開始X座標
  int startY = 100;              // 開始Y座標
  int width = 100;               // 幅
  int height = 50;               // 高さ
  uint16_t fillColor = TFT_BLUE; // 塗りつぶし色

  M5.Lcd.fillRect(startX, startY, width, height, fillColor);
  M5.Lcd.println("Hello World!");

  pinMode(35, INPUT);
}

const int inputPin = 35; // デジタル入力ピン

void loop()
{
  if (digitalRead(inputPin) == LOW)
  {
    while (digitalRead(inputPin) == LOW)
    {
      NOP();
    }
    uint32_t timeRise1 = micros();
    while (digitalRead(inputPin) == HIGH)
    {
      NOP();
    }
    uint32_t timeFall1 = micros();
    while (digitalRead(inputPin) == LOW)
    {
      NOP();
    }
    uint32_t timeRise2 = micros();
    float freq = 1.0 / ((timeRise2 - timeRise1) * 1e-6);
    float duty = double(timeFall1 - timeRise1) / double(timeRise2 - timeRise1);
    delay(100);

    int startX = 0;                           // 開始X座標
    int startY = 0;                           // 開始Y座標
    int width = 320;                          // 幅
    int height = 240;                         // 高さ
    uint16_t fillColor = HVtoRGB565(duty, 1); // 塗りつぶし色

    M5.Lcd.fillRect(startX, startY, width, height, fillColor);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.printf("freq: %f Hz\nduty: %f\npulsewidth: %d(us)", freq, duty, timeFall1 - timeRise1);
  }
}